from PIL import Image
from pathlib import Path
import imghdr
import os
result = open("sources/Fonts.cpp", "w")
resultEnum = open("include/SSD1351_Fonts_enum.h", "w")
resultEnum.write("#ifndef _SSD1351_FONTS_ENUM_\n#define _SSD1351_FONTS_ENUM_\n\nenum FontIndex{\n")
result.write("#include \"SSD1351_Fonts.h\"\n\n\nnamespace fnt{\n")
p = Path("Fonts")
fonts = [x for x in p.iterdir() if x.is_dir()]
for f in range(len(fonts)):
    sizes = [x for x in fonts[f].iterdir() if x.is_dir()]
    for i in range(len(sizes)):
        s = fonts[f].name + sizes[i].name
        result.write("    const bool " + s + "Alphabet[NUMBER_OF_CHARACTERS][")
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
                for k in range(round(len(greenPositions)/2)):
                    widths.append(greenPositions[2*k+1][0]- greenPositions[2*k][0] - 1)
                    heights.append(greenPositions[2*k+1][1]- greenPositions[2*k][1] - 1)
                for k in range(len(bluePositions)):
                    xPositions.append(bluePositions[k][0] - greenPositions[2*k][0])
                    yPositions.append(bluePositions[0][1] - bluePositions[k][1] + greenPositions[2*k][1] + 1)
                result.write(str(max(widths)*max(heights))+ "] =\n")
                result.write("        {\n")
                for k in range(len(bluePositions)):
                    result.write("            {")
                    for m in range(greenPositions[2*k][1] + 1, greenPositions[2*k+1][1]):
                        for l in range(greenPositions[2*k][0] + 1, greenPositions[2*k+1][0]):
                            if alphabetR.getpixel((l, m)):
                                result.write("true")
                            else:
                                result.write("false")
                            if l==greenPositions[2*k+1][0] - 1 and m== greenPositions[2*k+1][1] - 1:
                                result.write("}")
                                if k==len(bluePositions) - 1:
                                    result.write("\n        };\n\n")
                                else:
                                    result.write(",\n")
                            else:
                                result.write(", ")
                alphabetR.close()
                result.write("    const uint8_t " + s + "Widths[] = {\n        ")
                for x in range(len(widths)):
                    result.write(str(widths[x]))
                    if x==len(widths) - 1:
                        result.write("\n    };\n\n")
                    else:
                        result.write(", ")
                result.write("    const uint8_t " + s +"Heights[] = {\n        ")
                for x in range(len(heights)):
                    result.write(str(heights[x]))
                    if x==len(heights) - 1:
                        result.write("\n    };\n\n")
                    else:
                        result.write(", ")  
                result.write("    const int8_t " + s + "XPositions[] = {\n      ")
                for x in range(len(xPositions)):
                    result.write(str(xPositions[x]))
                    if x==len(xPositions) - 1:
                        result.write("\n    };\n\n")
                    else:
                        result.write(", ")
                result.write("    const int8_t " + s + "YPositions[] = {\n       ")
                for x in range(len(yPositions)):
                    result.write(str(yPositions[x]))
                    if x==len(yPositions) - 1:
                        result.write("\n    };\n\n")
                    else:
                        result.write(", ")
                result.write("    Font " + s + "= Font(" + str(max(widths)*max(heights)) +", &(" + s + "Alphabet[0][0]), " + s + "Widths, " + s + "Heights, " + s + "XPositions, " + s + "YPositions);\n" )
result.write("    Font fontList[] = {\n    ")
counter = 0
for i in range(len(fonts)):
    sizes = [x for x in fonts[f].iterdir() if x.is_dir()]
    for j in range(len(sizes)):
        resultEnum.write("    " + fonts[i].name.upper() + sizes[j].name + " =    " + str(counter))
        counter = counter + 1
        result.write("    " + fonts[i].name + sizes[j].name)
        if i == len(fonts) - 1 and j == len(sizes) - 1:
            resultEnum.write("\n}; \n")
            result.write("\n    };\n\n")
        else:
            resultEnum.write(", \n")
            result.write(", ")
result.write("    FontDatabase fontDatabase = fontList;\n")
result.write("    const uint8_t fontSizes[] = {\n")
for i in range(len(fonts)):
    sizes = [x for x in fonts[f].iterdir() if x.is_dir()]
    for j in range(len(sizes)):
        result.write("        " + sizes[j].name)
        if j == len(sizes) - 1 and i == len(fonts) - 1:
            result.write("\n    };\n")
        else:
            result.write(", ")
resultEnum.write("\n\n#endif\n")
resultEnum.close()
result.write("}\n")
result.close()

        
