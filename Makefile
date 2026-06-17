CC     := g++
TESTPY := --repository-url https://test.pypi.org/legacy/
LIBS   := -I./src -L./src -lm
CFLAGS := -std=c++14 -g -Wall -O3
CXX    := $(CC) $(LIBS) $(CFLAGS)

HEADER := src/gcxmlib.h
SOURCE := $(wildcard src/*.cc)
OBJECT := $(patsubst %.cc,%.o,$(SOURCE))
EGG_INFO := gcxmlib.egg-info
BUILD_DIR := build
BUILD_LIB := $(BUILD_DIR)/lib
BUILD_TEMP := $(BUILD_DIR)/temp
DIST_DIR := $(BUILD_DIR)/dist

EXAMPLE := test/example_angle \
           test/example_longitude \
           test/example_latitude \
           test/example_vector3 \
           test/example_vector3_outer_product \
           test/example_matrix3 \
           test/example_matrix3_operator \
           test/example_matrix3_chol \
           test/example_dcos \
           test/example_dcos_extend_to \
           test/example_footprint \
           test/example_footprint_neighbor_to \
           test/example_footprint_match \
           test/example_footprint_extend_to \
           test/example_great_circle_dump \
           test/example_great_circle_separation \
           test/example_great_circle_foot_of \
           test/example_minor_arc \
           test/example_minor_arc_distance \
           test/example_trail \
           test/example_trail_separation \
           test/example_trail_intersect_with_point \
           test/example_trail_intersect_with_arc \
           test/example_trail_colinear_with_gc \
           test/example_trail_colinear_with_arc \
           test/example_trail_propagate \
           test/example_trail_match \
           test/example_trajectory \
           test/example_trajectory_propagate

.PHONY: clean build example test serve demo

all: $(EXAMPLE)

test/example_%: test/example_%.cc $(OBJECT) $(HEADER)
	$(CXX) -o $@ $< $(OBJECT)

%.o: %.cc $(HEADER)
	$(CXX) -o $@ -c $<

build: gcxmlib.pyx
	python setup.py build_ext \
		--build-lib $(BUILD_LIB) --build-temp $(BUILD_TEMP)
	python -m build --outdir $(DIST_DIR)

demo: build
	python -c 'import gcxmlib; gcxmlib.simple_demo()'

example: $(EXAMPLE)

clean:
	rm -rf $(BUILD_DIR) $(EGG_INFO) $(OBJECT) $(EXAMPLE)

serve:
	mkdocs serve
