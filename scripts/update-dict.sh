#!/usr/bin/env bash
# Updates the dictionary files defined in extras/dict.json.

BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

EXTRAS_DIR=${BASE_DIR}/../extras
CONFIG=${EXTRAS_DIR}/dict.json

for lang in $(jq -r 'keys[]' $CONFIG); do

	echo "- Generating: $lang"

	files=$(jq -r ".$lang | if type==\"array\" then .[] else . end" $CONFIG)
	dict=$(grep -hs ^ $files | $BASE_DIR/generate-dict.sh > $EXTRAS_DIR/dict/$lang)
done

echo "Done"
