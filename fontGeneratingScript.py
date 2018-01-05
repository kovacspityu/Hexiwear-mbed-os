from PIL import Image
from pathlib import Path

result = open(Path("/sources/Fonts.cpp"), "a")
p = Path("/Fonts")
for font in p.iterdir():
    if font.isdir():
        sizes = [x in font.iterdir(), x.isdir()]
        for i in range(len(sizes)):
            result.write("Font ")
            result.write(font.string())
            result.write((i+5).string())
            result.write(";\n")
        for i in range(len(sizes)):
            result.write(font.string())
            result.write((i+5).string())
            result.write(".alphabet = {")
            for j in Path(size[i]).iterdir():
                if j.isfile():
                    letter = Image.open(j).getchannel("R")
                    for h in range(letter.size[0]):
                        for w in range(letter.size[1]): # or viceversa, check!
                            pixel = letter.getpixel((w, h))
                            if pixel:
                                result.write("true, ")
                            else:
                                result.write("false, ")
                    letter.close()
            result.write("\b\b")
            result.write("};\n\n")
        for i in range(len(sizes)):
            result.write(font.string())
            result.write((i+5).string())
            result.write(".width = {\n")
            for j in Path(size[i]).iterdir():
                if j.isfile():
                    letter = Image.open(j).getchannel("R")
                    result.write(letter.size[0])
                    letter.close()
                    result.write(", \n")
            result.write("\b\b\b")
            result.write("};\n\n")
        for i in range(len(sizes)):
            result.write(font.string())
            result.write((i+5).string())
            result.write(".height = {")
            for j in Path(size[i]).iterdir():
                if j.isfile():
                    letter = Image.open(j).getchannel("R")
                    result.write(letter.size[1])
                    letter.close()
                    result.write(", \n")
            result.write("\b\b\b")
            result.write("};\n\n")
        for i in range(len(sizes)):
            result.write(font.string())
            result.write((i+5).string())
            result.write(".xPosition = {")
            for j in Path(size[i]).iterdir():
                if j.isfile():
                    letter = Image.open(j).getchannel("B")
                    for h in range(letter.size[0]):
                        for w in range(letter.size[1]): # or viceversa, check!
                            if letter.getpixel((w, h)):
                                result.write(w.string())
                    letter.close()
                    result.write(", \n")
            result.write("\b\b\b")
            result.write("};\n\n")
        for i in range(len(sizes)):
            result.write(font.string())
            result.write((i+5).string())
            result.write(".yPosition = {")
            for j in Path(size[i]).iterdir():
                if j.isfile():
                    letter = Image.open(j).getchannel("B")
                    for h in range(letter.size[0]):
                        for w in range(letter.size[1]): # or viceversa, check!
                            if letter.getpixel((w, h)):
                                result.write(h.string())
                    letter.close()
            result.write(", \n")
            result.write("\b\b\b")
            result.write("};\n\n")

        