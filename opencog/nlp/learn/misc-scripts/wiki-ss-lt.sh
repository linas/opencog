#!/bin/bash
#
# Batch parsing script for Lithuanian.
# Loop over all the files in 'beta-pages', sentence-split them
# and submit them for parsing.
#
time find beta-pages -type f -exec ./ss-one.sh lt \"{}\" localhost 17002 \;
