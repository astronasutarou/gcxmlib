#!/usr/bin/env cython
# -*- coding: utf-8 -*-
from cython.operator cimport dereference as deref
from libcpp cimport bool
from libcpp.vector cimport vector as vec
from libcpp.set cimport set as cset
from libcpp.list cimport list as clist
from libcpp.string cimport string as cstring
from libcpp.memory cimport unique_ptr, make_unique
from libcpp.memory cimport shared_ptr, make_shared
from numpy cimport ndarray
from astropy.time import Time, TimeDelta
from astropy.coordinates import Longitude, Latitude, Angle, SkyCoord
from dataclasses import dataclass
import numpy as np


cdef extern from '<initializer_list>' namespace 'std':
    cdef cppclass initializer_list[T]:
        pass


cdef extern from '<chrono>' namespace 'std::chrono':
    ## import system_clock
    cdef cppclass system_clock:
        pass
    ## import time_point
    cdef cppclass time_point[Clock,Duration=*]:
        pass
    ## import duration
    cdef cppclass duration[Rep, Period=*]:
        ctypedef Rep rep
        duration() except +
        Rep2 duration[Rep2](const Rep2&) except +
        rep count() except +


cdef extern from 'gcxmlib.h' namespace 'gcxmlib':
    ## define an alias to duration[double]
    ctypedef duration[double] sec_t
    ## define an alias to system_clock
    ctypedef system_clock default_clock
    ## define an alias to time_point[default_clock]
    ctypedef time_point[default_clock] timestamp_t
    ## define an alias to vec[vec[double]]
    ctypedef vec[vec[double]] dump_array

    ## convert timestamp to std::string
    cdef const cstring timestamp_to_string(const timestamp_t ts)
    ## advance timestamp_t with duration
    cdef const T advance_timestamp[T](const timestamp_t ts, const T dt)

    ## general purpose angle class
    cdef cppclass angle:
        angle(const double arg)
        const double radian, degree, arcmin, arcsec

    ## angle class for Longitude values
    cdef cppclass longitude:
        longitude(const double arg)
        const double radian, degree, arcmin, arcsec

    ## angle class for Latitude values
    cdef cppclass latitude:
        latitude(const double arg) except +
        const double radian, degree, arcmin, arcsec

    cdef const angle radian(const double arg)
    cdef const angle degree(const double arg)
    cdef const angle arcmin(const double arg)
    cdef const angle arcsec(const double arg)

    cdef cppclass footprint:
        footprint(
            const double l, const double m, const double n,
            const timestamp_t ts, const angle sigma)
        footprint(
            const longitude lon, const latitude lat,
            const timestamp_t ts, const angle sigma)
        const longitude lon
        const latitude lat
        const timestamp_t t
        const angle s
        void print(const int N) const

    cdef cppclass trail:
        trail(const footprint& s, const footprint& e)
        const footprint propagate(const sec_t& dT) const
        const footprint propagate(const timestamp_t& T) const
        const bool match(const trail& arc,
            const angle& dtol = degree(5.0),
            const angle& rtol = arcmin(5.0),
            const double margin = 1.0) const
        const footprint s, e
        const sec_t dt
        void print(const int N) const
        void print_arc(const int N) const
        void print_error(const int N) const
        const dump_array dump(const int N) const
        const dump_array dump_arc(const int N) const
        const dump_array dump_error(const int N) const

    cdef cppclass trajectory:
        trajectory(const trail& arc)
        trajectory(const initializer_list[trail] arcs)
        trajectory(const vec[trail] arcs)
        void print(const int N=64) const
        void print_arc(const int N) const
        void print_error(const int N) const
        const dump_array dump(const int N) const
        const dump_array dump_arc(const int N) const
        const dump_array dump_error(const int N) const

    cdef const timestamp_t generate_timestamp(
        const int year, const int month, const int day,
        const int hour, const int minute, const int second,
        const int microsecond)


cdef extern from *:
    pass


ctypedef shared_ptr[longitude] lon_ptr
ctypedef shared_ptr[latitude]    lat_ptr
ctypedef shared_ptr[footprint] footprint_ptr
ctypedef shared_ptr[trail] trail_ptr
ctypedef shared_ptr[trajectory] trajectory_ptr


cdef lon_ptr wrap_longitude(lon: Longitude):
    cdef double radian = lon.radian
    return make_shared[longitude](radian)


cdef lat_ptr wrap_latitude(lon: Latitude):
    cdef double radian = lon.radian
    return make_shared[latitude](radian)


cdef footprint_ptr generate_footprint(fp: Footprint):
    return make_shared[footprint](
        deref(wrap_longitude(fp.lon)), deref(wrap_latitude(fp.lat)),
        get_timestamp(fp.ts), radian(fp.sig.radian))


cdef timestamp_t get_timestamp(t: Time):
    y,m,d,H,M,S = t.ymdhms
    uS,S = [int(np.round(s*x)) for s,x in zip(np.modf(S),(1e6,1))]
    return generate_timestamp(y,m,d,H,M,S,uS)


cdef dump_timestamp(const timestamp_t T):
    cdef cstring timestr = timestamp_to_string(T)
    return Time(timestr)


cdef dump_footprint(const footprint& fp):
    lon = Longitude(fp.lon.degree, unit='degree')
    lat = Latitude(fp.lat.degree, unit='degree')
    ts    = dump_timestamp(fp.t)
    sig = Angle(fp.s.degree, unit='degree')
    return Footprint(lon, lat, ts, sig)

##############

def __benchmark__(func):
    def wrap(*args, **options):
        t0 = Time.now()
        print('## start time: {}'.format(t0.isot))
        func(*args, **options)
        print('## elapsed time {} ms'.format((Time.now()-t0).sec*1000))
    return wrap


@dataclass
class Footprint:
    ''' Footprint class '''
    lon: Longitude
    lat: Latitude
    ts:    Time
    sig: Angle

    ## workaround until Cython version 3.0
    __annotations__ = {
        'lon': Longitude, 'lat': Latitude, 'ts': Time, 'sig': Angle}

    @staticmethod
    def from_xyz(x: float, y: float, z: float, ts: Time, sig: Angle):
        pos = SkyCoord(x=x,y=y,z=z,
                        representation_type='cartesian').represent_as('spherical')
        return Footprint(pos.lon,pos.lat,ts,sig)


cdef class Trail:
    cdef trail_ptr tp
    def __cinit__(self, s: Footprint, e: Footprint):
        self.tp = make_shared[trail](
            deref(generate_footprint(s)), deref(generate_footprint(e)))

    @property
    def start(self):
        return dump_footprint(deref(self.tp).s)

    @property
    def end(self):
        return dump_footprint(deref(self.tp).e)

    def propagate(self, T: Time):
        return dump_footprint(deref(self.tp).propagate(get_timestamp(T)))

    def dump_circle(self, N: int=64):
        return np.array(deref(self.tp).dump(N))

    def dump_arc(self, N: int=64):
        return np.array(deref(self.tp).dump_arc(N))

    def dump_error(self, N: int=64):
        return np.array(deref(self.tp).dump_error(N))

    def dump_timestamp(self):
        return dump_timestamp(deref(self.tp).s.t)


@__benchmark__
def simple_demo():
    lon1 = Longitude(20.0, unit='degree')
    lat1 = Latitude(-15.0, unit='degree')
    ts1    = Time.now()
    sig1 = Angle(1.0, unit='arcsec')

    lon2 = Longitude(180.0, unit='degree')
    lat2 = Latitude(60.0, unit='degree')
    ts2    = ts1 + TimeDelta(150.0, format='sec')
    sig2 = Angle(1.0, unit='arcsec')

    fp1 = Footprint(lon1, lat1, ts1, sig1)
    fp2 = Footprint(lon2, lat2, ts2, sig2)
    print(fp1)
    print(fp2)

    arc = Trail(fp1, fp2)
    print(arc)

    import matplotlib.pyplot as plt
    fig = plt.figure(figsize=(8,6))
    ax = fig.add_subplot()
    alon, alat = [], []
    for v in arc.dump_arc(360):
        f = Footprint.from_xyz(v[0],v[1],v[2],ts1,sig1)
        alon.append(f.lon.degree)
        alat.append(f.lat.degree)
    ax.scatter(alon, alat, 1)
    ax.scatter(fp1.lon.degree, fp1.lat.degree, 20)
    ax.scatter(fp2.lon.degree, fp2.lat.degree, 20)

    tsx = ts2
    for i in range(5):
        tsx = tsx + TimeDelta(10.0, format='sec')
        p = arc.propagate(tsx)
        ax.scatter(p.lon.degree, p.lat.degree, 30)

    ax.set_xlim([0,360])
    ax.set_ylim([-30,90])
    ax.grid(True)
    plt.show()
