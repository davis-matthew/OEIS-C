/*
 * A277193  Number of integers k in range [n^2, ((n+1)^2)-1] for which 3 = the
 *          least number of squares that add up to k (A002828).
 * https://oeis.org/A277193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 102, u32 n <= 512, u64 n <= 512,
 *            u128 n <= 512, big any n
 * Helpers:   floor_sqrt, is_four_four_eight_seven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A277193: Number of integers k in [n², (n+1)²−1] requiring exactly three squares.
   Formula: a(n) = ∑_{k=n²}^{(n+1)²-1} (1-A010052(k))·(1-A229062(k))·(1-A072401(k))
   (equivalently, k is not a square, not representable as sum of two squares,
   and not of the form 4^a·(8b+7)). */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_four_four_eight_seven.h"

/* Test whether k is a perfect square */
static bool is_square(seq_t k)
{
    seq_t r = floor_sqrt(k);
    return r * r == k;
}

/* Test whether k can be expressed as a sum of two squares.
   By Legendre–Gauss theorem: a number n > 0 is representable iff in its prime
   factorisation each prime p ≡ 3 (mod 4) occurs with an even exponent. */
static bool is_sum_two_squares(seq_t n)
{
    /* Remove powers of 2 – they do not affect the condition */
    while ((n & 1) == 0)
        n >>= 1;

    seq_t p = 3;
    seq_t limit = floor_sqrt(n);
    while (p <= limit)
    {
        unsigned exp = 0;
        while (n % p == 0)
        {
            n /= p;
            exp ^= 1;                 /* toggle parity of exponent */
        }
        if (exp && (p & 3) == 3)       /* odd exponent of a prime 3 mod 4 */
            return false;
        if (n == 1)
            break;
        p += 2;
        limit = floor_sqrt(n);
    }
    /* Remaining prime > sqrt(original) */
    if (n > 1 && (n & 3) == 3)
        return false;
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 + 1;
    seq_t start = n0 * n0;
    seq_t end   = n1 * n1 - 1;

    seq_t count = 0;
    for (seq_t k = start; k <= end; ++k)
    {
        if (is_square(k))
            continue;                   /* requires 1 square */
        if (is_sum_two_squares(k))
            continue;                   /* requires 2 squares */
        if (is_four_four_eight_seven(k))
            continue;                   /* requires 4 squares */
        ++count;                        /* exactly 3 squares */
    }
    return count;
}
