include Rules.make
TOP_DIRECTORY = $(shell pwd)

all: build_src build_tests

PACKAGE_VERSION=4.3.2

version:
	sed "s/VERSION \"[0-9]\+.[0-9]\+.[0-9]\+\"/VERSION \"$(PACKAGE_VERSION)\"/1" src/version.cpp > src/version.cpp.new
	mv -f src/version.cpp.new src/version.cpp
	sed "s/^Version:[ \t]\+[0-9]\+.[0-9]\+.[0-9]\+/Version: $(PACKAGE_VERSION)/1" dtack-base-cpp.spec > dtack-base-cpp.spec.new
	mv -f dtack-base-cpp.spec.new dtack-base-cpp.spec

clean: clean_src clean_tests
	find . -name "core.*" -exec rm -f {} \;

build_src:
	@mkdir -p lib
	cd src; make

clean_src:
	cd src; make clean

build_tests:
	@mkdir -p bin
	cd tests; make

clean_tests:
	cd tests; make clean

DOCKER_IMAGE_NAME=dtack-base-cpp
docker_build:
	docker build -t $(DOCKER_IMAGE_NAME) .

docker_up:
	docker-compose up --build

docker_exec:
	docker exec -it $(DOCKER_IMAGE_NAME) /bin/bash

docker_push: docker_build
	docker tag $(DOCKER_IMAGE_NAME) docker.dtack.com/$(DOCKER_IMAGE_NAME):latest
	docker push docker.dtack.com/$(DOCKER_IMAGE_NAME):latest

GITLAB_CI_TESTS_STDERR=gitlab_ci_tests.stderr
gitlab_ci_tests: 
	rm -f $(GITLAB_CI_TESTS_STDERR)
	-LD_LIBRARY_PATH=lib bin/test_version 2>> $(GITLAB_CI_TESTS_STDERR)
	-LD_LIBRARY_PATH=lib bin/test_asserts 2>> $(GITLAB_CI_TESTS_STDERR)
	-LD_LIBRARY_PATH=lib bin/test_incidents 2>> $(GITLAB_CI_TESTS_STDERR)
	-LD_LIBRARY_PATH=lib bin/test_packets 2>> $(GITLAB_CI_TESTS_STDERR)
	-LD_LIBRARY_PATH=lib bin/test_configuration 2>> $(GITLAB_CI_TESTS_STDERR)

test_version:
	@echo "------------------------------------------------------"
	cd src; make
	cd tests; make version
	LD_LIBRARY_PATH=lib \
	bin/test_version

test_asserts:
	@echo "------------------------------------------------------"
	cd src; make
	cd tests; make asserts
	LD_LIBRARY_PATH=lib \
	bin/test_asserts

run-test-incidents:
	@echo "------------------------------------------------------"
	cd src; make
	cd tests; make incidents
	LD_LIBRARY_PATH=lib \
	bin/test_incidents

test_packets:
	@echo "------------------------------------------------------"
	cd src; make
	cd tests; make packets
	LD_LIBRARY_PATH=lib \
	bin/test_packets

test_configuration:
	@echo "------------------------------------------------------"
	cd src; make
	cd tests; make configuration
	LD_LIBRARY_PATH=lib \
	bin/test_configuration

11-bytes_reader.run:
	@echo "------------------------------------------------------"
	cd exercises; make 11-bytes_reader.build
	LD_LIBRARY_PATH=../dtack-base-cpp-64/lib \
	bin/11-bytes_reader

# Place the rpm build files on the shared volume.
RPM_ROOT = /root/workspace/build_rpm
# Name and version of rpm package.
PACKAGE_BASE = dtack-base-cpp
PACKAGE_NAME = $(PACKAGE_BASE)-$(PACKAGE_VERSION)
# Tarball that is used as the source of the build_rpm (mimics Jenkins convention).
TARBALL = $(RPM_ROOT)/SOURCES/$(PACKAGE_NAME).tar.gz

CLONE_DIRECTORY = $(RPM_ROOT)/clone/$(PACKAGE_BASE)
RPM_FILENAME = $(RPM_ROOT)/RPMS/x86_64/$(PACKAGE_NAME)-1.el7.maxlab.x86_64.rpm
build_rpm_development:
	mkdir -p $(CLONE_DIRECTORY)
	cp -rdpa ./* $(CLONE_DIRECTORY)
	cd $(CLONE_DIRECTORY); make clean; make build_rpm
	rpm -qpR $(RPM_FILENAME)

build_rpm:
	cd 
	# Clean binary artifacts in the current directory.
	make clean
	# Clean a partial rmpbuild.
	rm -rf $(RPM_ROOT)/SOURCES/$(PACKAGE_BASE)*
	rm -rf $(RPM_ROOT)/BUILD/$(PACKAGE_BASE)*
	mkdir -p $(RPM_ROOT)/SOURCES
	mkdir -p $(RPM_ROOT)/BUILD
	# Zip the current working directory.
	tar czf $(TARBALL) --transform 's,^\.,$(PACKAGE_NAME),' .
	# Do the build.
	rpmbuild --define "_topdir $(RPM_ROOT)" -tb $(TARBALL)
	mkdir -p ./RPMS
	cp $(RPM_ROOT)/RPMS/x86_64/*.rpm ./RPMS

gitlab_ci_publish_testing:
	scp ./RPMS/*.rpm jenkins@w-v-repo-3:/repo/testing/el7/x86_64/
	ssh jenkins@w-v-repo-3 createrepo --update /repo/testing/el7/x86_64

gitlab_ci_publish_public:
	scp ./RPMS/*.rpm jenkins@w-v-repo-3:/repo/public/el7/x86_64/
	ssh jenkins@w-v-repo-3 createrepo --update /repo/public/el7/x86_64
