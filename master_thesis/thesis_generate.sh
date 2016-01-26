#!/bin/sh
platex cover.tex;dvipdfmx cover.dvi
platex main.tex;platex main.tex;dvipdfmx main.dvi

pdftk cover.pdf main.pdf cat output master_thesis.pdf

evince master_thesis.pdf &
#open main.pdf &
