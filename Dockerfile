############################################################
# Dockerfile which should be used to compile the code and build the rpm.
############################################################

FROM centos:7

RUN yum -y install \
    epel-release \
    yum -y clean all

RUN yum -y install \
    gcc-c++ \
    make \
    rpm-build && \
    yum -y clean all

COPY . /app

