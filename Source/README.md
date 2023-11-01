# 64-bit ARX-box Alzette

Sparkle uses the 64-bit non-linear ARX-box Alzette.

(U, V) <-- (ZL, ZR)
u = u + (v >>> 31)
u = v ^ (u >>> 24)
