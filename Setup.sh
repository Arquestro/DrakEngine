#!/bin/bash

set -e

cd "`dirname "$0"`"

if [ "$(uname)" = "Darwin" ]; then
    # Setup premake
    cd ..
    premake_dir="vendor/premake/bin"
    if [ ! -f $premake_dir/premake5 ]; then
        mkdir -p $premake_dir
        cd $premake_dir
        premake_version="5.0.0-beta1"
        premake_archive_file="premake-$premake_version-macosx.tar.gz"
        echo $premake_archive_file
        curl -L -o $premake_archive_file https://github.com/premake/premake-core/releases/download/v5.0.0-beta1/$premake_archive_file
        tar -xf $premake_archive_file
        rm $premake_archive_file
        curl -o LICENSE.txt https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt
        cd "`dirname "$0"`"
    fi
else
	echo "Setup.sh ERROR: Only MacOS platform is supported for DrakEngine!"
	exit 1
fi
