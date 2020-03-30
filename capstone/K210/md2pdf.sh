#!/bin/bash

echo "Converting all Markdown to HTML" 
current="$(pwd)" # Get current directory
css='<head>
        <link rel="stylesheet" href="pandoc.css">
    </head>'
echo $css
cd "$(dirname "$(readlink -f "$0")")" 
cd "$1" # Cd into the directory

shopt -s nullglob
for file in *.md; # Convert to git .md flavored
do
    pandoc -f markdown $file -t gfm-o ${file%.*}.md # convert to pandoc markdown, not really nessary 
    if [ -f "${file%.*}.md" ]; then echo "The file ${file%.*} was created"; fi
done
#--------
for file in *.md; # Convert to .html
do
    pandoc $file -o ${file%.*}.html # convert to html
    if [ -f "${file%.*}.html" ]; then echo "The file ${file%.*} was created"; fi
    echo "Adding css styling"
    echo $css >> ${file%.*}.html
done
#-------
for file in *.html; 
do
    weasyprint $file ${file%.*}.pdf # create pdf
     if [ -f "${file%.*}.pdf" ]; then echo "The file ${file%.*}  was created"; fi 
done
#-------
eval "$(pandoc --bash-completion)"
echo "- Deleting html files";
rm *.html