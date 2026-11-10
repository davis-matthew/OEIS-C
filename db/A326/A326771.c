/*
 * A326771  Array T(x, y), x, y >= 0, read by increasing x+y and then
 *          increasing x: if A326764(x, y, z) = 0 for some z >= 0, then T(x,
 *          y) = z, else T(x, y) = -1.
 * https://oeis.org/A326771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   greedy search. A326764 is the 3D "queens" mex array, filled in
 *            order of increasing x+y+z, then x, then y. A cell is 0 exactly
 *            when no earlier zero lies on one of the 13 queen lines through
 *            it (axes, face diagonals, body diagonals). This code finds that
 *            set of zeros and returns, for (x,y), the z w...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *att_ = NULL;
static int *zero_z_ = NULL;
static long L_ = 0;

static void build_zeros(long Ln)
{
    long s, x, y, z, i;
    int dx, dy, dz;
    if (att_) free(att_);
    if (zero_z_) free(zero_z_);
    att_ = (unsigned char *)calloc((size_t)(Ln * Ln * Ln), 1);
    zero_z_ = (int *)malloc(sizeof(int) * (size_t)(Ln * Ln));
    for (i = 0; i < Ln * Ln; i++) zero_z_[i] = -1;
    L_ = Ln;
    for (s = 0; s < Ln; s++) {
        for (x = 0; x <= s; x++) {
            for (y = 0; y <= s - x; y++) {
                z = s - x - y;
                if (att_[(x * Ln + y) * Ln + z]) continue;
                /* new zero */
                zero_z_[x * Ln + y] = (int)z;
                for (dx = -1; dx <= 1; dx++)
                    for (dy = -1; dy <= 1; dy++)
                        for (dz = -1; dz <= 1; dz++) {
                            long cx, cy, cz;
                            if (dx == 0 && dy == 0 && dz == 0) continue;
                            cx = x + dx; cy = y + dy; cz = z + dz;
                            while (cx >= 0 && cy >= 0 && cz >= 0 &&
                                   cx < Ln && cy < Ln && cz < Ln) {
                                att_[(cx * Ln + cy) * Ln + cz] = 1;
                                cx += dx; cy += dy; cz += dz;
                            }
                        }
            }
        }
    }
}

seq_t a(idx_t n)
{
    idx_t s = 0, k;
    long x, y;
    while ((s + 1) * (s + 2) / 2 <= n) s++;
    k = n - s * (s + 1) / 2;
    x = (long)k;
    y = (long)(s - k);
    if (L_ == 0) build_zeros(64);
    for (;;) {
        if (x + y < L_ && zero_z_[x * L_ + y] >= 0)
            return (seq_t)zero_z_[x * L_ + y];
        build_zeros(L_ * 2);
    }
}
