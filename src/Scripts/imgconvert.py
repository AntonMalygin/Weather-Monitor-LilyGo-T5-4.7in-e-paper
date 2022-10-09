#!/usr/bin/python3

"""

Конвертирование изображения в массив иконок в формат исходного кода на c
Глубина цвта 4 бита на пиксель (16 градаций серого)

Зависимости:

python3 -m pip install pillow

Примеры использования:

Простое конвертирование, подразумевается что картинка шириной в один тайл: 

python ./imgconvert.py ./img_weathers.bmp iml_64x64x4

С масштабированием размера:

python ./imgconvert.py ./img128x128.bmp iml_64x64x4 --width 64 --height 2048

Исходная картинка в виде тайлов 16 x 2:

python ./imgconvert.py ./img_icons.bmp iml_24x24x4  --col 16 --row 2

"""

from PIL import Image, ImageOps
from argparse import ArgumentParser
import sys
import math
import struct


parser = ArgumentParser(
    description='Конвертирование изображения в массив иконок в формат исходного кода на c')
parser.add_argument('imgfile', type=str, help='Имя файла изображения')
parser.add_argument('name', type=str, help='Имя константы')
parser.add_argument('-o', '--outfile',  action="store", dest="out_file", help='Имя выходного файла' )
parser.add_argument('-c', '--col', action="store", dest="col", default=1, type=int)
parser.add_argument('-r', '--row' ,action="store", dest="row", default=1, type=int)
parser.add_argument('-w', '--width',action="store", dest="width", default=0, type=int)
parser.add_argument('-H', '--height', action="store", dest="height", default=0, type=int)

args = parser.parse_args()

img_file = args.imgfile
print("img file:", img_file)

var_name = args.name
print("var name:", var_name)

out_file = args.out_file
if out_file == None:
    out_file = img_file + '.c'
print("out file:", out_file)

num_col = args.col
num_row = args.row

new_width  =  args.width
new_height =  args.height

img = Image.open(img_file)
img_width  =  img.size[0]
img_height =  img.size[1]

print("img size:", img_width, img_height)

# convert to grayscale
img = img.convert(mode='L')

# Масштабирование изображение
if (new_width > 0) and (new_height > 0):
    print("Convert size:", new_width, new_height)
    img.thumbnail((new_width, new_height), Image.ANTIALIAS)
    img_width  =  img.size[0]
    img_height =  img.size[1]
    
if img_width % 2:
    print("image width must be even!", file=sys.stderr)
    sys.exit(1)

# Вычисление размера ячеек
tail_width  = math.ceil(img_width /  num_col)
tail_height = math.ceil(img_height /  num_row)

if (num_col > 1) or (num_row > 1):
    print("col count", num_col)
    print("row count", num_row)
    print("tail width", tail_width)
    print("tail height", tail_height)

# Write out the output file.

f = open(out_file, 'w')

data_size = img_width * img_height

f.write("const uint8_t {}[{}] = {{\n".format(var_name, math.ceil(data_size / 2)))

for row in range(0, num_row):
    for col in range(0, num_col):
        byte  = 0
        count = 0
        done = True
        for y in range(0, tail_height):
            for x in range(0, tail_width):
                iy = y + row * tail_height
                ix = x + col * tail_width
                l =  l = img.getpixel((ix, iy))
                if x % 2 == 0:
                    byte = l >> 4
                    done = False;
                else:
                    byte |= l & 0xF0
                    f.write("0x{:02x},".format(byte))
                    done = True
            f.write("\n");
        f.write("\n");
f.write("};\n")
f.close()
