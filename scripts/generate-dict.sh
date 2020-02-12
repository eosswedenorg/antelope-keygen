#!/usr/bin/env bash
# Generates a dictionary file for usage with eosio-keygen
#  - input is read from stdin and should have words separated by newline.
#  - output is written to stdout.

BASE58_ALPHABET=123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz

# 1. Remove any line that contains non base58 characters
# 2. Remove lines that are 2 or less characters long
# 3. Convert to lowercase
# 4. Sort and remove duplicate lines.
LC_CTYPE=C awk "! /[^${BASE58_ALPHABET}]/" < /dev/stdin 2> /dev/null \
	| sed -r '/^.{,2}$/d' \
	| tr '[:upper:]' '[:lower:]' \
	| sort | uniq -d
