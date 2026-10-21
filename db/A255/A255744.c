/*
 * A255744  a(1) = 1; for n > 1, a(n) = 10*9^(A000120(n-1)-1).
 * https://oeis.org/A255744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=10*9^(A000120(n-1)-1) for n>1 where A000120(m) is
 *            the popcount (number of 1‐bits) of m
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t m = n - 1;
    /* popcount of m */
    unsigned pc = __builtin_popcountll((unsigned long long)m);

    /* exponent = pc - 1 (pc is at least 1 for n>1) */
    seq_t exp = (seq_t)(pc - 1);

    /* 9^exp */
    seq_t pow9 = big_pow((seq_t)9, exp);

    /* a(n) = 10 * 9^exp */
    return (seq_t)10 * pow9;
}
