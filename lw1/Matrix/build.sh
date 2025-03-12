#!/bin/bash
TARGET=


while getopts "t:" opt; do
  case $opt in
    t)
      TARGET=$OPTARG
      /home/yel/.local/share/JetBrains/Toolbox/apps/clion/bin/cmake/linux/x64/bin/cmake --build ./cmake-build-debug --target "$TARGET"
      ;;
    *)
      exit 1
      ;;
  esac
done


