#!/bin/sh

# Ensure that we have appropriate versions of all packages

PKGS="gtk+-3.0 evince-document-3.0 evince-view-3.0 libgtkhtml-4.0"
for p in $PKGS; do
	pkg-config --exists $p
	if [ $? -gt 0 ]; then
		echo "package $p not found"
		exit 1
	fi
done

