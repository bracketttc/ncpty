#!/usr/bin/env python3

# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

import subprocess, os

# sphinx-build variables
project = "ncpty"
author = "Timothy Brackett"
copyright = "2020, Timothy Brackett"
extensions = ["breathe"]
breathe_projects = {"ncpty": "../build/xml"}
breathe_default_project = "ncpty"


# readthedocs.io build process
read_the_docs_build = os.environ.get("READTHEDOCS", None) == "True"
if read_the_docs_build:
    subprocess.call(
        "cd .. && mkdir -p build && cd build && cmake .. -DDOC_ONLY_BUILD:Bool=True && make doc",
        shell=True,
    )
