#!/bin/bash
pushd ..
vendor/premake/bin/premake5 clion --file=premake5.lua
popd
