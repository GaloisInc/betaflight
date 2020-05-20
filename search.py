import os
import sys
import argparse
import re
from os.path import join, getsize
import mmap
import subprocess
counter = 0
filenames = []
search_str = sys.argv[1]
def search(base_dir):

    for entry in os.scandir(base_dir):
        if entry.is_file() and not entry.is_symlink() and not entry.name.endswith(".md") and not entry.name.endswith(".pdf") and not entry.name.endswith(".zip"):
            fo = open(entry.path,encoding = "ISO-8859-1")
            #print(entry.name)
            # Read the first line from the file
            line = fo.readline()
            # Initialize counter for line number
            line_no = 1
            # Loop until EOF
            while line != '' :
                # Search for string in line
                index = line.find(search_str)
                if ( index != -1) :
                    if entry.name in filenames:
                        pass
                    else:
                        filenames.append(entry.name)
                        print(entry.path,line_no)
                #Read next line
                line = fo.readline()
                #Increment line counter
                line_no += 1
            fo.close()

        elif entry.is_dir():
            yield from search(entry.path)
        else:
            pass



for name in search("./"):
    # print(name)
    pass
