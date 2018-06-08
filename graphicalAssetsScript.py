from PIL import Image
from pathlib import Path
import sys
import imghdr
import os
outputHeader = open("include/gAssets.h", "w")
outputSource = open("sources/gAssets.cpp", "w")
outputHeader.write("#ifndef _GASSETS_\n#define _GASSETS_\n#include \"mbed.h\"\n\nnamespace gfx{\n")
outputSource.write("#include \"mbed.h\"\n#include \"gAssets.h\"\nnamespace gfx{\n")
p = Path("graphicalAssets")
images = [x for x in p.iterdir() if x.is_file() and imghdr.what(str(x))]
aux_str = ""
for x in range(15, -1, -1):
    aux_str = aux_str  + "{" + str(x) + "}"
for imageFile in images:
    print("Analyzing file " + imageFile.name)
    outputHeader.write("    extern const uint16_t " + imageFile.stem.lower() + "[];\n")
    imageR = Image.open(imageFile).getchannel("R")
    imageG = Image.open(imageFile).getchannel("G")
    imageB = Image.open(imageFile).getchannel("B")
    box = []
    if imageR.getbbox():
        box.append(imageR.getbbox())
    if imageG.getbbox():
        box.append(imageG.getbbox())
    if imageB.getbbox():
        box.append(imageB.getbbox())
    #print(str(box) + "\n")
    coordinates = [max([x[i] for x in box], default = 0) for i in range(4)]
    #print(str(coordinates) + "\n")
    width = coordinates[2] - coordinates[0]
    height = coordinates[3] - coordinates[1]
    outputSource.write("    const uint16_t " + imageFile.stem.lower() + "[" + str(width) + "*" + str(height) + "+" + "2] = {\n        ")
    outputSource.write(str(width) + ", " + str(height) + ", ")
    for j in range(coordinates[1], coordinates[3]):
        for i in range(coordinates[0], coordinates[2]):
            pixel = ((imageR.getpixel((i, j))>>3)<<11) | ((imageG.getpixel((i, j))>>2)<<5) | ((imageB.getpixel((i, j))>>3))
            pixel = int.from_bytes(pixel.to_bytes(2, byteorder = 'big'), byteorder = 'little')
            outputSource.write(str(pixel))
            if i!=coordinates[2]-1 or j!=coordinates[3]-1:
                outputSource.write(", ")
    outputSource.write("\n    };\n")
outputSource.write("}\n")
outputHeader.write("}\n\n#endif\n")
outputHeader.close()
outputSource.close()
