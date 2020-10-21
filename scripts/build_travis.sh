#!/usr/bin/env bash

set -ev

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

if [[ ("$TRAVIS_BRANCH" = "development") || ("$TRAVIS_BRANCH" = "release") ]]; then
    PYTHONPATH="test/system" pytest ../test/system
else
    echo "Skipping system tests for branch '$TRAVIS_BRANCH'"
fi
