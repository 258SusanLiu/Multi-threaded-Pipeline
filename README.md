# Multi-threaded-Pipeline

Code is written in C. Program is used to get famililar with threads, mutual exclusion and condition variables. This code uses the Producer-Consumer approach.

Thread 1 (Input Thread) - reads in lines of characters from the standard input.

Thread 2 (Line Separator Thread) - replaces every line separator in the input by a space.

Thread 3 (Plus Sign thread) - replaces every pair of plus signs, i.e., "++", by a "^".

Thread 4, (Output Thread) - writes this processed data to standard output as lines of exactly 80 characters.
