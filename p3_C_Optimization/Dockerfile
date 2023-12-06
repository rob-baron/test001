FROM c-fortran-dev

# Here is the project layer, here we just clone the repo
# however for development we will just mount the local directory
# over the appropriate directory.
WORKDIR /App
RUN cd /App \
    && git clone https://github.com/rob-baron/test001.git \
    && cd test001

ENTRYPOINT [ bash ]
