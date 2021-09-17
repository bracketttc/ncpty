FROM gcc:latest

ARG COV_VERSION=latest

RUN apt-get update && apt-get install -y curl

COPY coverity /coverity

ENV PATH=/coverity/bin:$PATH
