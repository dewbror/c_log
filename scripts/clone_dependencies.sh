#!/bin/bash

# List of repos to clone
REPOS=(
    "https://github.com/clibs/cmocka.git:1.1.5"
)

# Dir to clone repos into
TARGET_DIR="external"

# Create target dir if it doesnt exist
mkdir -p "$TARGET_DIR"

# Iterate over the list of repos and clone them
for entry in "${REPOS[@]}"; do
    # Split entry into repo and tag
    repo="${entry%.git:*}"
    tag="${entry##*.git:}"
    dir="$TARGET_DIR/$(basename "$repo" .git)"
    
    # If no colon, tag == repo
    if [[ "$repo" == "$tag" ]]; then
        tag="master"
    fi

    if [ -d "$dir" ]; then
        # If the repo already exists, do nothing
        echo "Directory $dir already exists"
    else
        # If the repo doesnt exist, clone it
        cd "$TARGET_DIR" || exit 1
        if git -c advice.detachedHead=false clone --recursive --depth 1 --branch "$tag" "$repo"; then
            echo "Switching to tag ${tag}"
        else
            git -c advice.detachedHead=false clone --recursive --depth 1 "$repo"
        fi
        cd .. || exit 1
    fi
done
