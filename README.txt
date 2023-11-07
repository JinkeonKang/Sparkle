This repository is created for the paper "New Distinguishing Attacks on Round-Reduced Sparkle384 and Sparkle512 Permutations."

We performed experiments using reduced variants of Alzette with input sizes of 16, 24, and 32 bits. In Section III-A, two probabilities, p and q, for differential properties of Alzette were calculated.

Table 2 shows success ratios ̃p and ̃q in the experiments for reduced Alzette with the block size b = 16, 24, and 32.
b     p                 q
16    36,679/100,000    63,168/100,000
24    3,847/10,000      6,382/10,000
32    376/1,000         629/1,000

In each trial of the experiments for p, we randomly chose two nonzero values as ∆ and ∇ and tested whether there exists an input pairs (z, z′) satisfying (1). In each trial of the experiments for q, we randomly chose two nonzero values as ∆ and ∇ and two different values as k and k′ and tested whether there exists an input pairs (z, z′) satisfying (2). 

The rotation amounts (31, 17, 0, 24, 24, 17, 31, 16) in Alzette were replaced with 
(7, 5, 0, 6, 6, 5, 7, 4) in 16-bit-block variant, 
(11, 7, 0, 9, 9, 7, 11, 6) in 24-bit-block variant, and 
(15, 9, 0, 12, 12, 9, 15, 8) in 32-bit-block variant, respectively.

The success ratio of reduced Alzette (b=16, p):
36679/100000 (30.872000 sec)
The success ratio of reduced Alzette (b=16, q):
63168/100000 (54.987000 sec)
The success ratio of reduced Alzette (b=24, p):
3847/10000 (1800.958000 sec)
The success ratio of reduced Alzette (b=24, q):
6382/10000 (3100.948000 sec)
The success ratio of reduced Alzette (b=32, p):
376/1000 (20317.696000 sec)
The success ratio of reduced Alzette (b=32, q):
629/1000 (39085.183000 sec)
