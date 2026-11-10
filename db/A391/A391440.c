/*
 * A391440  Number of genera in the order of real quadratic fields with
 *          discriminant A079896(n).
 * https://oeis.org/A391440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(omega(|D|) - t), D = A079896(n) (n-th non-square D ==
 *            0,1 mod 4, D>1); t = 0 if 32|D, t = 2 if D == 4 (mod 16), t = 1
 *            otherwise. Computed by scan + trial division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9345 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t found = 0, d = 1, r = 1;
    while (found < (uint64_t)n) {
        d++;
        if ((d & 3) > 1) continue;
        while (r * r < d) r++;
        if (r * r == d) continue;
        found++;
    }
    uint64_t t = d;
    int w = 0;
    if ((t & 1) == 0) {
        w++;
        while ((t & 1) == 0) t >>= 1;
    }
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            w++;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) w++;
    int tt;
    if (d % 32 == 0) tt = 0;
    else if (d % 16 == 4) tt = 2;
    else tt = 1;
    int e = w - tt;
    if (e < 0) e = 0;
    seq_t res = 1;
    for (int i = 0; i < e; i++) res = res * 2;
    return res;
}
