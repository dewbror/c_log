#!/bin/bash

# List of repos to clone
REPOS=(
    "https://github.com/clibs/cmocka.git"
)

REPO_TAGS=(
    1.1.5
)

# Dir to clone repos into
TARGET_DIR="external"

# Create target dir if it doesnt exist
mkdir -p "$TARGET_DIR"

i=0
# Iterate over the list of repos and clone them
for repo in "${REPOS[@]}"; do
    dir="$TARGET_DIR/$(basename "$repo" .git)"

    if [ -d "$dir" ]; then
        # If the repo already exists, do nothing
        echo "Directory $dir already exists"
    else
        # If the repo doesnt exist, clone it
        cd "$TARGET_DIR" || exit 1
        if git -c advice.detachedHead=false clone --recursive --depth 1 --branch "${REPO_TAGS[$i]}" "$repo"; then
            if [ "${REPO_TAGS[$i]}" != "master" ]; then
                echo "Note: switching to tag ${REPO_TAGS[$i]}"
            fi
        fi
        cd .. || exit 1
    fi
    i=$((i + 1))
done
