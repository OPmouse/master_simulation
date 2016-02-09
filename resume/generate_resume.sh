#!/bin/sh

platex template.tex;platex template.tex;dvipdfmx template.dvi;open template.pdf

