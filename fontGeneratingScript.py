from PIL import Image
from pathlib import Path
import imghdr
import os
result = open("sources/Fonts.cpp", "w")
result.write("#include \"SSD1351_Fonts.h\"\n\n\n")
p = Path("Fonts")
fonts = [x for x in p.iterdir() if x.is_dir()]
for font in fonts:
    sizes = [x for x in font.iterdir() if x.is_dir()]
    for i in range(len(sizes)):
        result.write("Character " + font.name + sizes[i].name + "Alphabet ={\n")
        for j in Path(sizes[i]).iterdir():
            if j.is_file() and imghdr.what(str(j)):
                print("Analyzing file", j.name)
                alphabetR = Image.open(j).getchannel("R")
                alphabetG = Image.open(j).getchannel("G")
                alphabetB = Image.open(j).getchannel("B")
                greenPositions = [(x, y) for x in range(alphabetG.width) for y in range(alphabetG.height) if alphabetG.getpixel((x, y))]
                bluePositions = [(x, y) for x in range(alphabetB.width) for y in range(alphabetB.height) if alphabetB.getpixel((x, y))]
                greenPositions.sort()
                bluePositions.sort()
                alphabetG.close()
                alphabetB.close()
                xPositions = [] 
                yPositions = []
                widths = []
                heights = []
                for k in range(len(bluePositions)):
                    xPositions.append(bluePositions[k][0] - greenPositions[2*k][0])
                    yPositions.append(bluePositions[k][1] - greenPositions[2*k][1])
                for k in range(round(len(greenPositions)/2)):
                    widths.append(greenPositions[2*k+1][0]- greenPositions[2*k][0])
                    heights.append(greenPositions[2*k+1][1]- greenPositions[2*k][1])
                for k in range(len(bluePositions)):
                    result.write("    {")
                    for l in range(greenPositions[2*k][0], greenPositions[2*k+1][0]):
                        for m in range(greenPositions[2*k][1], greenPositions[2*k+1][1]):
                            if alphabetR.getpixel((l, m)):
                                result.write("true")
                            else:
                                result.write("false")
                            if l==greenPositions[2*k+1][0] - 1 and m==greenPositions[2*k+1][1] - 1: 
                                result.write("}")
                                if k==len(bluePositions) - 1:
                                    result.write("\n};\n\n")
                                else:
                                    result.write(", \n")
                            else:
                                result.write(", ")
                alphabetR.close()
                result.write(font.name + sizes[i].name + "Widths = {\n    ")
                for x in range(len(widths)):
                    result.write(str(widths[x]))
                    if x==len(widths) - 1:
                        result.write("\n};\n\n")
                    else:
                        result.write(", ")
                result.write(font.name + sizes[i].name + "Heights = {\n    ")
                for x in range(len(heights)):
                    result.write(str(heights[x]))
                    if x==len(heights) - 1:
                        result.write("\n};\n\n")
                    else:
                        result.write(", ")  
                result.write(font.name + sizes[i].name + "XPositions = {\n    ")
                for x in range(len(xPositions)):
                    result.write(str(xPositions[x]))
                    if x==len(xPositions) - 1:
                        result.write("\n};\n\n")
                    else:
                        result.write(", ")
                result.write(font.name + sizes[i].name + "YPositions = {\n    ")
                for x in range(len(yPositions)):
                    result.write(str(yPositions[x]))
                    if x==len(yPositions) - 1:
                        result.write("\n};\n\n")
                    else:
                        result.write(", ")
        result.write("Font " + font.name + sizes[i].name + "= Font(" + font.name + sizes[i].name + "Alphabet, " + font.name + sizes[i].name + "Widths, " + font.name + sizes[i].name + "Heights, " + font.name + sizes[i].name + "XPositions, " + font.name + sizes[i].name + "YPositions);\n\n")
    result.write("Font " + font.name +"[] = {\n    ")
    for i in range(len(sizes)):
        result.write(font.name + sizes[i].name)
        if i == len(sizes) - 1:
            result.write("\n};\n\n")
        else:
            result.write(", ")
    result.write("FontList " + font.name + " = FontList(" + str(len(sizes)) + ", " + font.name + ");\n")
result.write("FontList fontList[] = {\n    ")
for i in range(len(fonts)):
    result.write(fonts[i].name)
    if i == len(fonts) - 1:
        result.write("\n};\n\n")
    else:
        result.write(", ")
result.write("FontDatabase fontDatabase = FontDatabase(" + str(len(fonts)) + ", fontList);\n")
result.close()

        
