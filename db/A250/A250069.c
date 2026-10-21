/*
 * A250069  a(n) = n^2 mod gpf(n^2 + 1) where gpf(k) is the greatest prime
 *          dividing k.
 * https://oeis.org/A250069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod gpf(n^2 + 1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* n is at most 2^32-1 for which n^2 fits in 64 bits. */
    uint64_t n64 = (uint64_t)n;
    uint64_t n_sq = n64 * n64;          /* n^2 */
    uint64_t p   = gpf(n_sq + 1);       /* greatest prime factor of n^2+1 */
    return (seq_t)n_sq % (seq_t)p;      /* n^2 mod p */
}
