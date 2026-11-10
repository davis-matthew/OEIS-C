/*
 * A326773  Array T(y, z), y, z >= 0, read by increasing y+z and then
 *          increasing y: if A326764(x, y, z) = 0 for some x >= 0, then T(y,
 *          z) = x, else T(y, z) = -1.
 * https://oeis.org/A326773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   search. Cells (x,y,z) of A326764 are scanned by increasing
 *            x+y+z, then x, then y. A cell gets the value 0 when no earlier 0
 *            lies on any of the 13 lines through it (directions are vectors
 *            whose nonzero components all have the same absolute value).
 *            T(y,z) is the x of the zero on the line (.,y,z); a...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 74 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static long long *t_memo = NULL;
static long long t_memoD = -1;

/* fills out[0..(D+1)(D+2)/2-1]; returns 1 if every needed line got its zero within sum <= S */
static int t_compute(long long D, long long S, long long *out)
{
    long long W = 2 * S + 1;
    size_t sz = (size_t)W * (size_t)W;
    unsigned char *tab = (unsigned char *)calloc(13 * sz, 1);
    long long need = (D + 1) * (D + 2) / 2, found = 0, i, s, x, y;
    int ok = 0;
    for (i = 0; i < need; i++) out[i] = -1;
    if (!tab) return 0;
    for (s = 0; s <= S && found < need; s++) {
        for (x = 0; x <= s && found < need; x++) {
            for (y = 0; y <= s - x; y++) {
                long long z = s - x - y;
                long long u[13], v[13];
                size_t idx[13];
                int t, free_cell = 1;
                u[0] = y;     v[0] = z;
                u[1] = x;     v[1] = z;
                u[2] = x;     v[2] = y;
                u[3] = x - y; v[3] = z;
                u[4] = x + y; v[4] = z;
                u[5] = x - z; v[5] = y;
                u[6] = x + z; v[6] = y;
                u[7] = y - z; v[7] = x;
                u[8] = y + z; v[8] = x;
                u[9] = x - y; v[9] = x - z;
                u[10] = x - y; v[10] = x + z;
                u[11] = x + y; v[11] = x - z;
                u[12] = x + y; v[12] = x + z;
                for (t = 0; t < 13; t++) {
                    long long a = u[t] + S, b = v[t] + S;
                    if (a < 0) a = 0;            /* cannot happen; safety */
                    if (a >= W) a = W - 1;
                    if (b < 0) b = 0;
                    if (b >= W) b = W - 1;
                    idx[t] = (size_t)t * sz + (size_t)a * (size_t)W + (size_t)b;
                    if (tab[idx[t]]) { free_cell = 0; break; }
                }
                if (!free_cell) continue;
                for (t = 0; t < 13; t++) tab[idx[t]] = 1;
                if (y + z <= D) {
                    long long d = y + z;
                    long long k = d * (d + 1) / 2 + y;
                    if (out[k] < 0) { out[k] = x; found++; }
                }
            }
        }
    }
    ok = (found == need);
    free(tab);
    return ok;
}

seq_t a(idx_t n)
{
    long long d = 0, y, k;
    while ((unsigned long long)((d + 1) * (d + 2) / 2) <= (unsigned long long)n) d++;
    if (d > t_memoD) {
        long long D = d < 16 ? 16 : d;
        long long need = (D + 1) * (D + 2) / 2;
        long long S = 4 * D + 32;
        long long *buf = (long long *)malloc((size_t)need * sizeof(long long));
        while (!t_compute(D, S, buf)) S *= 2;
        if (t_memo) free(t_memo);
        t_memo = buf;
        t_memoD = D;
    }
    y = (long long)n - d * (d + 1) / 2;
    k = d * (d + 1) / 2 + y;
    return (seq_t)t_memo[k];
}
