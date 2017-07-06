#!/bin/bash
make
valgrind --leak-check=full -v ./bin/battle
