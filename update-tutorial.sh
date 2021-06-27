#!/bin/sh

OUT="out"

if [ -d "$OUT" ]
then
  rm -rf "$OUT"
fi

mkdir "$OUT"
asciidoctor -v --section-numbers -D "$OUT" "*.adoc"
