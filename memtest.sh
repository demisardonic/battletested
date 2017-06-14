#!/bin/bash
make
valgrind --leak-check=full ./bin/battle