/*
 * A371461  Lexicographically latest sequence of distinct nonnegative integers
 *          such that for any n >= 0, A371442(a(n)) = A371442(n).
 * https://oeis.org/A371461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the class-reversal of n: with A371442(n) = v, the numbers
 *            k with A371442(k) = v form a finite set; a(n) is the one with
 *            the mirrored rank in increasing order (a(0) = 0)
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 83 terms through n = 9582 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    int w = 64 - __builtin_clzll((uint64_t)n);
    int wp = (w + 1) / 2;

    seq_t v = 0, f = 0;
    for (int i = 0; i < w; ++i) {
        int bit = (int)((n >> (w - 1 - i)) & 1ULL);
        if ((i & 1) == 0) v = (v << 1) | (seq_t)bit;
        else f = (f << 1) | (seq_t)bit;
    }

    seq_t half = (seq_t)1 << (wp - 1);
    seq_t r = f;
    if ((w & 1) == 0) r = r + half;
    seq_t K = half + ((seq_t)1 << wp);
    seq_t t = K - 1 - r;

    int w2;
    seq_t f2;
    if (t < half) {
        w2 = 2 * wp - 1;
        f2 = t;
    } else {
        w2 = 2 * wp;
        f2 = t - half;
    }
    int fb = w2 - wp;

    seq_t res = 0;
    int vi = 0, fi = 0;
    for (int i = 0; i < w2; ++i) {
        int bit;
        if ((i & 1) == 0) {
            bit = (int)((v >> (wp - 1 - vi)) & 1);
            ++vi;
        } else {
            bit = (int)((f2 >> (fb - 1 - fi)) & 1);
            ++fi;
        }
        res = (res << 1) | (seq_t)bit;
    }
    return res;
}
