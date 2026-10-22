/*
 * A372289  a(n) = n*2^e + (4^e - 1)/3, where e is the 2-adic valuation of n.
 * https://oeis.org/A372289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*2^e + (4^e - 1)/3, e = v2(n) (the 2‑adic valuation of
 *            n)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute e = v2(n) */
    idx_t e = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        e++;
        tmp >>= 1;
    }

    /* compute 2^e */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < e; ++i)
        pow2 *= 2;

    /* compute 4^e */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < e; ++i)
        pow4 *= 4;

    seq_t part = (pow4 - 1) / 3;
    return ((seq_t)n) * pow2 + part;
}
