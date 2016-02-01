#!/bin/sh

platex template.tex;platex template.tex;dvipdfmx template.dvi;evince template.pdf

