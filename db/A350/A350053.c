/*
 * A350053  a(n) = (2^(3*n + 3 + (-1)^n) - (6 + (-1)^n))/9, for n >= 1.
 * https://oeis.org/A350053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(3*n + 3 + (-1)^n) - (6 + (-1)^n)) / 9 , n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
#include "../utils/big_pow.h"

/* Computes the nth term of OEIS sequence A350053 */
seq_t a(idx_t n)
{
    /* n is unsigned: careful with bitwise operations */
    /* Compute (-1)^n : 1 for even, -1 for odd */
    /* For odd n: (-1)^n = -1 -> exp = 3*n + 2, sub = 5
     * For even n: (-1)^n =  1 -> exp = 3*n + 4, sub = 7  */
    idx_t exp = 3 * n + ((n & 1) ? (idx_t)2 : (idx_t)4);
    seq_t sub = (n & 1) ? (seq_t)5 : (seq_t)7;

    /* If seq_t fits in an unsigned integer, use 128‑bit arithmetic
     * to avoid overflow of 2^exp even when exp > 63 but ≤ 127.
     * For larger exp (≥ 128) the result would exceed any 64‑bit
     * type, and such values are not verified by the 64‑bit build. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* 2^exp fits in 128 bits for the n values tested with 64‑bit seq_t */
        unsigned __int128 pow2 = ((unsigned __int128)1) << exp;
        unsigned __int128 numerator = pow2 - (unsigned __int128)sub;
        unsigned __int128 res128 = numerator / (unsigned __int128)9;
        return (seq_t)res128;
    }

    /* Arbitrary‑precision build: rely on big_pow to generate 2^exp exactly */
    seq_t pow2 = big_pow((seq_t)2, exp);
    seq_t numerator = pow2 - sub;
    return numerator / (seq_t)9;
}
