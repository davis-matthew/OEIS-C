/*
 * A323642  Row n of triangle A323641 when n -> infinity.
 * https://oeis.org/A323642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3^{popcount(n)-1} if n is odd * a(n) =
 *            2*3^{popcount(n-1)-1} if n is even * (popcount counts the number
 *            of 1‑bits in the binary representation)
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t e;
    seq_t res = 1;

    /* popcount on 64‑bit unsigned int */
    if (n & 1ULL) {            /* n odd */
        e = __builtin_popcountll((uint64_t)n) - 1;
    } else {                   /* n even */
        e = __builtin_popcountll((uint64_t)(n - 1)) - 1;
    }

    /* compute 3^e */
    for (idx_t i = 0; i < e; ++i) {
        res *= 3;
    }

    /* multiply by 2 if n is even */
    if ((n & 1ULL) == 0) {
        res *= 2;
    }

    return res;
}
