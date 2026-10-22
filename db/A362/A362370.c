/*
 * A362370  Triangle read by rows. T(n, k) = ([x^k] P(n, x)) // k! where P(n,
 *          x) = Sum_{k=1..n} P(n - k, x) * x if n >= 1 and P(0, x) = 1. The
 *          notation 's // t' means integer division and is a shortcut for
 *          'floor(s/t)'.
 * https://oeis.org/A362370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = floor( binom(n-1,k-1) / k! ); a(n) is the flattened
 *            sequence of T(n,k) read by rows. For n = 0 use a(0) = 1
 * Range:     u8 n <= 59, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helpers for binomial coefficients and factorials. */
#include "../utils/binom.h"    /* seq_t binom(idx_t a, idx_t b);      */
#include "../utils/factorial.h"/* seq_t factorial(idx_t n);           */

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t
row_from_index(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Exponentially grow high until its start index exceeds n. */
    while ( ((unsigned __int128)high * (high + 1)) / 2 <= n )
        high <<= 1;

    /* Binary search between low (inclusive) and high (exclusive). */
    while (low + 1 < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if ( prod / 2 <= n )
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t
a(idx_t n)
{
    /* Compute row r and column k for the flattened index n. */
    idx_t r = row_from_index(n);
    unsigned __int128 start = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - (idx_t)start;   /* 0‑based column within the row */

    /* Handle the special base row. */
    if (r == 0)
        return (seq_t)1;

    /* For other rows, coefficient at column 0 is zero. */
    if (k == 0)
        return (seq_t)0;

    /* Compute floor( binom(r-1, k-1) / k! ). */
    seq_t binom_val = binom(r - 1, k - 1);   /* = C(r-1, k-1) */
    seq_t fact_val  = factorial(k);          /* = k! */
    return binom_val / fact_val;             /* integer division is floor */
}
