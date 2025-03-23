#!/bin/bash

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=DEBUG -S . -B ./build

ln -s $pwd/build/compile_commands.json $pwd/../compile_commands.json
