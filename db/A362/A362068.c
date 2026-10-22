/*
 * A362068  a(n) is the smallest positive integer k such that n can be
 *          expressed as the arithmetic mean of k squares.
 * https://oeis.org/A362068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   can_sum_two, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest k such that n can be expressed as the mean of k squares
   (i.e., n*k is a sum of k squares).  Uses the theorems on sums of 1, 2, 3 and 4 squares. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/can_sum_two.h"

/* Return true iff all primes p ≡ 3 (mod 4) divide n with even exponent. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1, but guard */

    /* 1 square (k=1): n must be a perfect square */
    seq_t s = floor_sqrt((seq_t)n);
    if (s * s == (seq_t)n)
        return (seq_t)1;

    /* 2 squares (k=2): n must have no prime ≡3 (mod 4) with odd exponent */
    if (can_sum_two(n))
        return (seq_t)2;

    /* 3 squares (k=3): n*3 must NOT be of the form 4^a(8b+7) */
    seq_t m = (seq_t)n * 3;
    while (m % 4 == 0)
        m /= 4;
    if (m % 8 == 7)        /* 3n is of forbidden form → need 4 squares */
        return (seq_t)4;

    return (seq_t)3;       /* otherwise 3 squares suffice */
}
