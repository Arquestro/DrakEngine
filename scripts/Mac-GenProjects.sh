#!/bin/bash
pushd ..
vendor/premake/bin/premake5 gmake --file=premake5.lua
popd
