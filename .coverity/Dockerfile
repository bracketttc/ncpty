FROM gcc:latest AS download

ARG COVERITY_TOKEN

RUN mkdir /coverity && \
    cd coverity && \
    wget --quiet https://scan.coverity.com/download/linux64 --post-data "token=${COVERITY_TOKEN}&project=bracketttc%2Fncpty" -O - | tar zx --strip-components 1

FROM gcc:latest

RUN apt-get update && apt-get install -y curl

COPY --from=download /coverity /coverity

ENV PATH=/coverity/bin:$PATH
