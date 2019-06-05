#!/bin/bash

if [[ "$#" == "0" ]];then
    filename=`echo *.tex`
else
    exit
fi

filename=${filename/.tex}


tmp="tmp"
rm -rf $tmp
mkdir -p $tmp

flags="-output-directory=$tmp -shell-escape"

export TEXINPUTS=".//:$TEXINPUTS"
export BIBINPUTS=".//:$BIBINPUTS"
export BSTINPUTS=".//:$BSTINPUTS"

xelatex $flags $filename || exit
bibtex ./$tmp/$filename
xelatex $flags $filename || exit
xelatex $flags $filename || exit

System_Name=`uname`
if [[ $System_Name == "Linux" ]]; then
    PDFviewer="gnome-open"
elif [[ $System_Name == "Darwin" ]]; then
    PDFviewer="open"
else
    PDFviewer="open"
fi

mv -f ./$tmp/"$filename".pdf ./"$filename".pdf

$PDFviewer ./"$filename".pdf || exit