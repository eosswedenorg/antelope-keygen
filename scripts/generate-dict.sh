#!/usr/bin/env bash
# Generates a dictionary file for usage with eosio-keygen
#  - input is read from stdin and should have words separated by newline.
#  - output is written to stdout.

BASE58_ALPHABET=123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz

# 1. Remove lines that are 2 or less characters long
# 2. Convert to lowercase
# 3. Remove any line that contains non base58 characters
# 4. Sort and remove duplicate lines.
LC_CTYPE=C sed -r '/^.{,2}$/d' < /dev/stdin 2> /dev/null \
	| tr '[:upper:]' '[:lower:]' \
	| awk "! /[^${BASE58_ALPHABET}]/" \
	| sort | uniq
