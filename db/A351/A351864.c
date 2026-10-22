/*
 * A351864  Numerator of zeta({6}_n)/Pi^(6n).
 * https://oeis.org/A351864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 2^(A000120(3*n + 1) - 1) (popcount of 3*n+1 minus 1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the exponent: popcount(3*n + 1) - 1 */
    uint64_t val = 3 * n + 1;                 /* 3*n+1 fits in 64 bits for n up to 64-bit range */
    unsigned int pc = __builtin_popcountll(val); /* popcount of 64-bit integer */
    unsigned int e = pc - 1;                  /* exponent, guaranteed non‑negative */

    /* Compute 2^e in seq_t */
    seq_t res = (seq_t)1;
    for (unsigned int i = 0; i < e; ++i)
        res *= 2;

    return res;
}
