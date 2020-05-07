#!/usr/bin/env python3

# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

import subprocess, os

extensions = ["breathe"]
breathe_projects = {"ncpty": "../build/xml"}
breathe_default_project = "ncpty"

read_the_docs_build = os.environ.get("READTHEDOCS", None) == "True"
if read_the_docs_build:
    subprocess.call(
        "cd .. && mkdir -p build && cd build && cmake .. && make doc", shell=True
    )
