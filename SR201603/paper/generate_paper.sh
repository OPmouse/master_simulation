#!/bin/sh

platex sr.tex;platex sr.tex;dvipdfmx sr.dvi;evince sr.pdf
