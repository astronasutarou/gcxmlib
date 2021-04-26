CC     := g++
TESTPY := --repository-url https://test.pypi.org/legacy/
LIBS   := -I./src -L./src -lm
CFLAGS := -std=c++14 -g -Wall -O3
CXX    := $(CC) $(LIBS) $(CFLAGS)

HEADER := src/spchord.h
SOURCE := $(wildcard src/*.cc)
OBJECT := $(patsubst %.cc,%.o,$(SOURCE))

EXAMPLE := test/example_angle \
           test/example_longitude \
           test/example_latitude \
           test/example_vector3 \
           test/example_dcos \
           test/example_source_neighbor_to \
           test/example_source_match

.PHONY: clean build test build_pypi upload_test upload_pypi

all: $(EXAMPLE)

test/example_%: test/example_%.cc $(OBJECT) $(HEADER)
	$(CXX) -o $@ $< $(OBJECT)

.cc.o: $(HEADER)
	$(CXX) -o $@ -c $<

build:
	python setup.py build_ext --inplace

test: $(EXAMPLE)
# test: build
# 	python -c 'import minimalKNN as m; m.simple_demo_box()'

build_pypi: build
	python setup.py sdist bdist_wheel -p manylinux1_x86_64

upload_test: build_pypi
	twine upload --skip-existing $(TESTPY) dist/*

upload_pypi: build_pypi
	twine upload --skip-existing dist/*

clean:
	rm -r $(OBJECT)
