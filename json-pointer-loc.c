#include <stdint.h>
#include <stdlib.h>
#include "json-pointer-loc.h"

/* -- adaptation of the musl libc implementation of wcwidth -- 
----------------------------------------------------------------------
Copyright © 2005-2020 Rich Felker, et al.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------
*/
static const char non_spacing_chars[] = {
    16,16,16,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,16,33,16,16,16,34,35,36,
    37,38,39,40,16,16,41,16,16,16,16,16,16,16,16,16,16,16,42,43,16,16,44,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,45,16,46,47,48,49,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,50,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,51,16,16,52,
    53,16,54,55,56,16,16,16,16,16,16,57,16,16,58,16,59,60,61,62,63,64,65,66,67,68,
    69,70,16,71,72,73,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,74,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,75,76,16,16,16,77,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,78,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,79,80,16,16,16,16,16,16,16,81,16,16,16,16,16,82,83,84,16,16,16,16,16,85,
    86,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,248,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,254,255,255,255,255,191,182,0,0,0,0,0,0,0,63,0,255,23,0,0,0,0,0,248,255,
    255,0,0,1,0,0,0,0,0,0,0,0,0,0,0,192,191,159,61,0,0,0,128,2,0,0,0,255,255,255,
    7,0,0,0,0,0,0,0,0,0,0,192,255,1,0,0,0,0,0,0,248,15,32,0,0,192,251,239,62,0,0,
    0,0,0,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,248,255,255,255,255,
    255,7,0,0,0,0,0,0,20,254,33,254,0,12,0,0,0,2,0,0,0,0,0,0,16,30,32,0,0,12,0,0,
    64,6,0,0,0,0,0,0,16,134,57,2,0,0,0,35,0,6,0,0,0,0,0,0,16,190,33,0,0,12,0,0,
    252,2,0,0,0,0,0,0,144,30,32,64,0,12,0,0,0,4,0,0,0,0,0,0,0,1,32,0,0,0,0,0,0,17,
    0,0,0,0,0,0,192,193,61,96,0,12,0,0,0,2,0,0,0,0,0,0,144,64,48,0,0,12,0,0,0,3,0,
    0,0,0,0,0,24,30,32,0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,4,92,0,0,0,0,0,0,0,0,0,0,0,
    242,7,128,127,0,0,0,0,0,0,0,0,0,0,0,0,242,31,0,63,0,0,0,0,0,0,0,0,0,3,0,0,160,
    2,0,0,0,0,0,0,254,127,223,224,255,254,255,255,255,31,64,0,0,0,0,0,0,0,0,0,0,0,
    0,224,253,102,0,0,0,195,1,0,30,0,100,32,0,32,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,224,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,28,0,0,0,28,0,0,0,12,0,0,0,12,0,0,0,0,0,0,0,176,63,64,254,
    15,32,0,0,0,0,0,120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,0,0,0,0,2,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,135,1,4,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    128,9,0,0,0,0,0,0,64,127,229,31,248,159,0,0,0,0,0,0,255,127,0,0,0,0,0,0,0,0,
    15,0,0,0,0,0,208,23,4,0,0,0,0,248,15,0,3,0,0,0,60,59,0,0,0,0,0,0,64,163,3,0,0,
    0,0,0,0,240,207,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,247,255,253,33,16,
    3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,
    251,0,248,0,0,0,124,0,0,0,0,0,0,223,255,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,
    255,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,3,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,0,0,
    0,60,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,128,247,63,0,0,0,192,0,0,0,0,0,0,0,0,0,0,3,0,68,8,0,0,96,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,48,0,0,0,255,255,3,128,0,0,0,0,192,63,0,0,128,255,3,0,
    0,0,0,0,7,0,0,0,0,0,200,51,0,0,0,0,32,0,0,
    0,0,0,0,0,0,126,102,0,8,16,0,0,0,0,0,16,0,0,0,0,0,0,157,193,2,0,0,0,0,48,64,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,33,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,0,0,0,
    64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,255,
    255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,110,240,0,
    0,0,0,0,135,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,0,0,0,0,0,0,0,240,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,255,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,255,127,0,0,0,0,0,0,128,
    3,0,0,0,0,0,120,38,0,32,0,0,0,0,0,0,7,0,0,0,128,239,31,0,0,0,0,0,0,0,8,0,3,0,
    0,0,0,0,192,127,0,30,0,0,0,0,0,0,0,0,0,0,0,128,211,64,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,128,248,7,0,0,3,0,0,0,0,0,0,24,1,0,0,0,192,31,31,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,255,92,0,0,64,0,0,0,0,0,0,0,0,0,0,248,133,13,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,60,176,1,0,0,48,0,0,0,0,0,0,0,0,0,0,
    248,167,1,0,0,0,0,0,0,0,0,0,0,0,0,40,191,0,0,0,0,0,0,0,0,0,0,0,0,224,188,15,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,255,6,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,240,12,1,0,0,0,254,7,0,0,0,0,248,121,128,0,126,14,0,0,0,0,0,252,
    127,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,127,191,0,0,0,0,0,0,0,0,0,0,252,255,
    255,252,109,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,126,180,191,0,0,0,0,0,0,0,0,0,163,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,255,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,0,0,0,0,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,0,0,0,128,7,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,15,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,3,248,255,231,15,0,0,0,60,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,
    255,255,255,255,127,248,255,255,255,255,255,31,32,0,16,0,0,248,254,255,0,0,0,
    0,0,0,0,0,0,0,127,255,255,249,219,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,240,7,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

static const char wide_chars[] = {
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,18,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,19,16,20,21,22,16,16,16,23,16,16,24,25,26,27,28,17,
    17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,29,
    17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
    17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
    17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
    17,17,17,17,17,17,17,17,30,16,16,16,16,31,16,16,17,17,17,17,17,17,17,17,17,17,
    17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
    17,17,17,17,17,17,17,32,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,17,17,16,16,16,33,
    34,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,35,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
    17,17,17,17,17,17,36,17,17,37,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,17,38,39,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
    16,16,16,16,16,16,16,40,41,42,43,44,45,46,47,16,48,49,16,16,16,16,
    16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12,0,6,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,30,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,0,0,48,0,0,0,0,0,0,255,15,0,0,0,0,128,0,0,8,
    0,2,12,0,96,48,64,16,0,0,4,44,36,32,12,0,0,0,1,0,0,0,80,184,0,0,0,0,0,0,0,224,
    0,0,0,1,128,0,0,0,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,33,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,251,255,255,255,255,255,255,255,
    255,255,255,15,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,63,0,0,0,255,15,255,255,255,255,
    255,255,255,127,254,255,255,255,255,255,255,255,255,255,127,254,255,255,255,
    255,255,255,255,255,255,255,255,255,224,255,255,255,255,255,254,255,255,255,
    255,255,255,255,255,255,255,127,255,255,255,255,255,7,255,255,255,255,15,0,
    255,255,255,255,255,127,255,255,255,255,255,0,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
    0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,31,255,255,255,255,255,255,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,
    255,255,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,15,0,0,0,0,0,0,0,0,0,0,0,0,0,255,3,0,0,255,255,255,255,247,255,127,15,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,254,255,255,255,255,255,255,255,255,255,255,
    255,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,15,0,0,0,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,7,0,255,255,255,127,0,0,0,0,0,
    0,7,0,240,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    15,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,64,254,7,0,0,0,0,0,0,0,0,0,0,0,0,7,0,255,255,255,
    255,255,15,255,1,3,0,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,
    1,224,191,255,255,255,255,255,255,255,255,223,255,255,15,0,255,255,255,255,
    255,135,15,0,255,255,17,255,255,255,255,255,255,255,255,127,253,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    159,255,255,255,255,255,255,255,63,0,120,255,255,255,0,0,4,0,0,96,0,16,0,0,0,
    0,0,0,0,0,0,0,248,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,
    255,255,255,255,255,255,63,16,39,0,0,24,240,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,255,15,0,
    0,0,224,255,255,255,255,255,255,255,255,255,255,255,255,123,252,255,255,255,
    255,231,199,255,255,255,231,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,15,7,7,0,63,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

static int wcwidth(uint32_t wc)
{
	if (wc < 0xffU)
		return (wc+1 & 0x7f) >= 0x21 ? 1 : wc ? -1 : 0;
	if ((wc & 0xfffeffffU) < 0xfffe) {
		if ((non_spacing_chars[non_spacing_chars[wc>>8]*32+((wc&255)>>3)]>>(wc&7))&1)
			return 0;
		if ((wide_chars[wide_chars[wc>>8]*32+((wc&255)>>3)]>>(wc&7))&1)
			return 2;
		return 1;
	}
	if ((wc & 0xfffe) == 0xfffe)
		return -1;
	if (wc-0x20000U < 0x20000)
		return 2;
	if (wc == 0xe0001 || wc-0xe0020U < 0x5f || wc-0xe0100U < 0xef)
		return 0;
	return 1;
}
/* -- end of adaptation of the musl libc implementation of wcwidth -- */

static inline const char* skip_whitespace(const char* sptr, const char* send, size_t* lc) {
    while (sptr != send && (
        *sptr == ' ' || 
        *sptr == '\t' || 
        *sptr == '\r' || 
        *sptr == '\n')
    ) {
        if (*sptr == '\n')
            (*lc)++;
        sptr++;
    }
    return sptr;
}

static inline const char* skip_pointer_token(const char* pptr, const char* pend) {
    while (pptr != pend) {
        if (*pptr == '~')
            pptr++;
        if (*pptr == '/')
            return pptr;
        pptr++;
    }
    return pend;
}

static const char* skip_member_name(const char* sptr, const char* send, size_t* lc) {
    sptr++; // Skip '"'
    while (sptr != send && *sptr != '"') {
        if (*sptr == '\\')
            sptr++; // Skip potential escaped quote. We don't care about unicode value so we can ignore that special case
        sptr++;
    }

    while (sptr != send && *sptr != ':') { // Skip to ':'
        if (*sptr == '\n')
            (*lc)++;
        sptr++;
    }

    if (sptr != send)
        sptr++; // Skip past ':'

    return skip_whitespace(sptr, send, lc); // Skip whitespace between ':' and value
}

static inline int is_hex(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c < 'f') || (c >= 'A' && c <= 'F');
}

static inline int is_unicode(char c) {
    return (unsigned char)c >= 0x80;
}

static inline int is_ptr_escape(char c) {
    return c == '~';
}

static inline int is_json_escape(char c) {
    return c == '\\';
}

static inline int is_simple_json(char c) {
    return !is_unicode(c) && !is_json_escape(c);
}

static inline int is_simple_ptr(char c) {
    return !is_unicode(c) && !is_ptr_escape(c);
}

static inline unsigned long decode_hex_quad(const char* c) {
    char hex[5];
    hex[0] = c[0];
    hex[1] = c[1];
    hex[2] = c[2];
    hex[3] = c[3];
    hex[4] = '\0';
    return strtol(hex, NULL, 16);
}

/* -- UTF-8 decoder -- */
/* Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de> */
/* See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details. */

#define UTF8_ACCEPT 0
#define UTF8_REJECT 12

static const uint8_t utf8d[] = {
  // The first part of the table maps bytes to character classes that
  // to reduce the size of the transition table and create bitmasks.
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
   8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

  // The second part is a transition table that maps a combination
  // of a state of the automaton and a character class to a state.
   0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
  12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
  12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
  12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
  12,36,12,12,12,12,12,12,12,12,12,12, 
};

static inline uint32_t decode(uint32_t* state, uint32_t* codep, uint32_t byte) {
  uint32_t type = utf8d[byte];

  *codep = (*state != UTF8_ACCEPT) ?
    (byte & 0x3fu) | (*codep << 6) :
    (0xff >> type) & (byte);

  *state = utf8d[256 + *state + type];
  return *state;
}
/* end of UTF-8 decoder */

static inline const char* decode_utf8(const char* sptr, const char* send, uint32_t* code) {
  uint32_t state = 0;
  while (sptr != send && decode(&state, code, (uint32_t)(uint8_t)*sptr))
    sptr++;
  return sptr == send ? send : sptr + 1;
}

static int validate_json_pointer(const char* pptr, const char* pend) {
    if (pptr != pend && *pptr != '/') 
        return JPLOC_INVALID_PTR;

    uint32_t code;
    uint32_t utf8_state = 0;
    int prev_is_tilde = 0;
    int ptr_is_valid = 1;
    while (pptr != pend) {
        decode(&utf8_state, &code, (unsigned char)*pptr);
        ptr_is_valid = ptr_is_valid && (!prev_is_tilde || *pptr == '0' || *pptr == '1');
        prev_is_tilde = *pptr == '~';
        pptr++;
    }
    return utf8_state == UTF8_ACCEPT && ptr_is_valid
        ? JPLOC_SUCCESS
        : JPLOC_INVALID_PTR;
}

static inline const char* decode_ptr_char(const char* pptr, const char* pend, uint32_t* code) {
    if (is_unicode(*pptr)) {
        return decode_utf8(pptr, pend, code);
    } else if (*pptr == '~') {
        *code = (uint32_t)(*pptr == '0' ? '~' : '/');
        return pptr + 2;
    } else {
        *code = (uint32_t)*pptr;
        return pptr + 1;
    }
}

static const char* decode_json_char(const char* c, const char* end, uint32_t* code) {
    if (is_unicode(*c)) {
        return decode_utf8(c, end, code);
    } else if (is_json_escape(*c)) {
        if (c + 1 == end) {
            *code = 0xFFFD;
            return end;
        }
        switch (c[1]) {
            case '"':
                *code = (uint32_t)'"';
                return c + 2;
            case '\\':
                *code = (uint32_t)'\\';
                return c + 2;
            case '/':
                *code = (uint32_t)'/';
                return c + 2;
            case 'b':
                *code = (uint32_t)'\b';
                return c + 2;
            case 'f':
                *code = (uint32_t)'\f';
                return c + 2;
            case 'n':
                *code = (uint32_t)'\n';
                return c + 2;
            case 'r':
                *code = (uint32_t)'\r';
                return c + 2;
            case 'u': {
                if (c + 6 >= end) {
                    *code = 0xFFFD;
                    return end;
                }
                unsigned long hex_code = decode_hex_quad(c + 2);
                if (hex_code >= 0xD800 && hex_code <= 0xDBFF) {
                    /* surrogate pair */
                    if (c + 6 >= end || c[6] != '\\' || c[7] != 'u') {
                        *code = 0xFFFD;
                        return c + 6;
                    }
                    if (c + 11 >= end) {
                        *code = 0xFFFD;
                        return end;
                    }
                    unsigned long lower_code = decode_hex_quad(c + 8);
                    if (lower_code >= 0xDC00 && lower_code <= 0xDFFF) {
                        *code = ((hex_code - 0xD800) << 10) + (lower_code - 0xDC00) + 0x10000;
                        return c + 12;
                    }
                }
                *code = hex_code;
                return c + 6;
            }
            default:
                *code = 0xFFFD;
                return c + 2;
        }
    } else {
        *code = (uint32_t)*c;
        return c + 1;
    }
}
    
static int compare_key(const char* sptr, const char* send, const char* pptr, const char* pend) {
    while (pptr != pend && *pptr != '\0' && *pptr != '/' && sptr != send && *sptr != '\0' && *sptr != '"') {
        if (is_simple_ptr(*pptr) && is_simple_json(*sptr)) {
            if (*pptr != *sptr)
                return 0;
            pptr++;
            sptr++;
        } else {
            uint32_t ptr_code;
            uint32_t json_code;
            pptr = decode_ptr_char(pptr, pend, &ptr_code);
            sptr = decode_json_char(sptr, send, &json_code);
            if (ptr_code != json_code)
                return 0;
        }
    }
    return (pptr == pend || *pptr == '\0' || *pptr == '/') && (sptr == send || *sptr == '\0' || *sptr == '\"');
}

static const char* skip_string(const char* sptr, const char* send) {
    sptr++; // Skip opening quote
    while (sptr != send && *sptr != '"') {
        if (*sptr == '\\') // No need to worry about content, just need to skip escaped quotes
            sptr++;
        sptr++;
    }
    if (sptr != send)
        sptr++; // Skip closing quote
    return sptr;
}

static const char* skip_member(const char* sptr, const char* send, size_t* lc) {
    size_t object_level = 0;
    size_t array_level = 0;
    while (sptr != send) {
        switch (*sptr) {
            case '"':
                sptr = skip_string(sptr, send);
                break;
            case '{':
                object_level++;
                sptr++;
                break;
            case '}':
                if (object_level == 0 && array_level == 0)
                    goto exit_loop;
                object_level--;
                sptr++;
                break;
            case '[':
                array_level++;
                sptr++;
                break;
            case ']':
                array_level--;
                sptr++;
                break;
            case ',':
                if (object_level == 0 && array_level == 0)
                    goto exit_loop;
                sptr++;
                break;
            case '\n':
                (*lc)++;
                sptr++;
                break;
            default:
                sptr++;
        }        
    }
    exit_loop:
    return sptr;
}

static const char* scan_to_key(const char* sptr, const char* send, const char* pptr, const char* pend, size_t* lc) {
    sptr = skip_whitespace(sptr, send, lc);
    while (sptr != send && *sptr != '}') {
        // Compare each property and either skip or return pointer to value
        if (compare_key(sptr + 1, send, pptr, pend)) {
            return skip_member_name(sptr, send, lc); // Move to JSON value
        } else {
            sptr = skip_member(sptr, send, lc); // Move to next member
            if (sptr != send && *sptr == ',') {
                sptr++;
                sptr = skip_whitespace(sptr, send, lc);
            }
        }
    }
    return NULL;
}

static int read_index(const char* pptr, const char* pend, size_t* index) {
    *index = 0;
    while (pptr != pend && *pptr >= '0' && *pptr <= '9') {
        *index = *index * 10 + ((size_t)*pptr - (size_t)'0');
        pptr++;
    }
    return pptr != pend && *pptr != '/';
}

static const char* scan_to_index(const char* sptr, const char* send, const char* pptr, const char* pend, size_t* lc) {
    size_t index;
    if (read_index(pptr, pend, &index))
        return NULL;

    size_t i;
    sptr = skip_whitespace(sptr, send, lc);
    for (i = 0; sptr != send && *sptr != ']' && i < index; i++) {
        sptr = skip_member(sptr, send, lc);
        if (sptr != send && *sptr == ',') {
            sptr++;
            sptr = skip_whitespace(sptr, send, lc);
        }
    }
    if (i == index && sptr != send && *sptr != ']')
        return sptr;
    else
        return NULL;
}

size_t compute_column(const char* start, const char* pos, const char* end) {
    const char* c = pos;
    if (c == start) 
        return 1;

    // Scan backwards to start of line
    while (c != start && *c != '\n') 
        c--;

    // Count code points and use wcwidth to compute the "width" of each one. 
    // This method does not consider grapheme clusters, but since VS Code 
    // doesn't appear to either, this is acceptable for reporting errors.
    uint32_t codepoint;
    uint32_t state = 0;
    int column = 1;
    for (; c < pos; ++c) {
        if (!decode(&state, &codepoint, (uint32_t)(uint8_t)*c)) {
            int width = wcwidth(codepoint);
            if (width > 0)
                column += width;
        }
    }

    return column;
}

int json_pointern_loc(
    const char* source, 
    size_t source_length,
    const char* pointer,
    size_t pointer_length,
    json_pointer_loc_t* location
) {
    if (source == NULL)
        return JPLOC_NULL_SOURCE;

    if (pointer == NULL)
        return JPLOC_NULL_PTR;

    const char* sptr = source;
    const char* send = source + source_length;
    const char* pptr = pointer;
    const char* pend = pointer + pointer_length;

    int pointer_err = validate_json_pointer(pptr, pend);
    if (pointer_err != JPLOC_SUCCESS)
        return pointer_err;

    size_t lc = 0; // Line count, to compute line/column information

    // Move to root value
    sptr = skip_whitespace(sptr, send, &lc);

    // Scan
    while (pptr != pend && sptr != send) {
        pptr++; // Skip '/'

        // Read next JSON token to determine type
        int result;
        switch (*sptr) {
            case '{':
                sptr++; // Skip '{'
                sptr = scan_to_key(sptr, send, pptr, pend, &lc);
                if (sptr == NULL)
                    return JPLOC_NOT_FOUND;
                break;
            
            case '[':
                sptr++;
                sptr = scan_to_index(sptr, send, pptr, pend, &lc);
                if (sptr == NULL) 
                    return JPLOC_NOT_FOUND;
                break;

            default:
                return JPLOC_NOT_FOUND; // scalar value
        }

        // Go to next token
        pptr = skip_pointer_token(pptr, pend);
    }
    if (pptr == pend) {
        location->line = lc + 1;
        location->column = compute_column(source, sptr, send);
        location->position = sptr - source;
        return 0;
    }
    return 1;
}

int json_pointer_loc(
    const char* source, 
    size_t source_length,
    const char* pointer,
    json_pointer_loc_t* location
) {
    if (pointer == NULL)
        return JPLOC_NULL_PTR;

    const char *pointer_end = pointer;
    const char *pointer_end_max = pointer + 1024; 
    while (pointer_end < pointer_end_max && *pointer_end)
        pointer_end++;
    int pointer_length = pointer_end - pointer;
    return json_pointern_loc(source, source_length, pointer, pointer_length, location);
}