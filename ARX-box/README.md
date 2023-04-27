# 64-bit ARX-box Alzette

Sparkle uses the 64-bit non-linear ARX-box Alzette.

Alzette(x, y) // x, y: 32-bit
x += ROT(y, 31)
y ^= ROT(x, 24)
x ^= c
x += ROT(y, 17)
y ^= ROT(x, 17)
x ^= c
x += ROT(y, 0)
y ^= ROT(x, 31)
x ^= c
x += ROT(y, 24)
y ^= ROT(x, 16)
x ^= c

