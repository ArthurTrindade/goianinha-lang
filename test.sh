#!/bin/bash

# Build the C++ program using the Makefile
echo "Building the program..."
make

# Check if the build was successful
if [ $? -eq 0 ]; then
  # Run the program
  echo "Running the program..."
  ./goianinha tests/main.g
else
  echo "Build failed. Please check for errors."
fi