/*
 * A326772  Array T(x, z), x, z >= 0, read by increasing x+z and then
 *          increasing x: if A326764(x, y, z) = 0 for some y >= 0, then T(x,
 *          z) = y, else T(x, z) = -1.
 * https://oeis.org/A326772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Build the zeros of A326764 greedily:
 *            visit the cells (x,y,z) by increasing x+y+z, and within that by
 *            increasing x, then y. A cell holds 0 when no earlier zero lies
 *            on any of the 13 lines through it (directions with components in
 *            {-1,0,1}). Then a(n) = T(x,z) = the y of...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static int g_S = 0;
static int *g_res = NULL;

static void run_sim(int S)
{
    int W = 3 * S + 1;
    size_t plane = (size_t)W * (size_t)W;
    unsigned char *f = (unsigned char *)calloc(13 * plane, 1);
    int *res = (int *)malloc(sizeof(int) * (size_t)(S + 1) * (size_t)(S + 1));
    int i, s, x, y, z;
    for (i = 0; i < (S + 1) * (S + 1); i++) res[i] = -1;
    for (s = 0; s <= S; s++) {
        for (x = 0; x <= s; x++) {
            for (y = 0; y <= s - x; y++) {
                int a[13], b[13];
                int hit = 0, d;
                z = s - x - y;
                a[0] = y;          b[0] = z;
                a[1] = x;          b[1] = z;
                a[2] = x;          b[2] = y;
                a[3] = x - y + S;  b[3] = z;
                a[4] = x + y;      b[4] = z;
                a[5] = x - z + S;  b[5] = y;
                a[6] = x + z;      b[6] = y;
                a[7] = y - z + S;  b[7] = x;
                a[8] = y + z;      b[8] = x;
                a[9] = x - y + S;  b[9] = x - z + S;
                a[10] = x - y + S; b[10] = x + z;
                a[11] = x + y;     b[11] = x - z + S;
                a[12] = x + y;     b[12] = x + z;
                for (d = 0; d < 13; d++) {
                    if (f[(size_t)d * plane + (size_t)a[d] * W + b[d]]) { hit = 1; break; }
                }
                if (!hit) {
                    for (d = 0; d < 13; d++)
                        f[(size_t)d * plane + (size_t)a[d] * W + b[d]] = 1;
                    res[x * (S + 1) + z] = y;
                }
            }
        }
    }
    free(f);
    if (g_res) free(g_res);
    g_res = res;
    g_S = S;
}

seq_t a(idx_t n)
{
    idx_t s = 0, x, z;
    int S;
    while ((s + 1) * (s + 2) / 2 <= n) s++;
    x = n - s * (s + 1) / 2;
    z = s - x;
    if (g_res && (idx_t)g_S >= s) {
        int r = g_res[(int)x * (g_S + 1) + (int)z];
        if (r >= 0) return (seq_t)r;
    }
    S = g_S > 0 ? g_S : 64;
    if ((idx_t)S < 4 * s) S = (int)(4 * s);
    for (;;) {
        if (S > g_S) run_sim(S);
        {
            int r = g_res[(int)x * (g_S + 1) + (int)z];
            if (r >= 0) return (seq_t)r;
        }
        S *= 2;
    }
}
