/*
 * A390278  All positive numbers except those that are the product of an even
 *          number of distinct primes (including 1).
 * https://oeis.org/A390278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390278: a(n) = the nth positive integer k (k ≥ 2) with Möbius(k) ≤ 0
   (i.e., k is not 1 and not the product of an even number of distinct primes). */

#include <stdlib.h>
#include <stdbool.h>

/* Möbius function for a 64‑bit integer.
   Returns  1 if k is square‑free with an even number of distinct primes,
   -1 if square‑free with an odd number of distinct primes,
    0 if k has a squared prime factor.
   For k = 1 the function returns 1 (so the number is excluded). */
static int mobius(idx_t k)
{
    if (k == 1) return 1;

    int distinct = 0;
    for (idx_t p = 2; p * p <= k; ++p)
    {
        if (k % p == 0)
        {
            ++distinct;
            k /= p;
            if (k % p == 0) /* squared factor */
                return 0;
            while (k % p == 0) /* remove any further copies (should not happen) */
                k /= p;
        }
    }
    if (k > 1) /* remaining prime factor */
        ++distinct;
    return (distinct & 1) ? -1 : 1;
}

/* Return true iff the integer k is included in the sequence
   (i.e., Möbius(k) ≤ 0). */
static bool included(idx_t k)
{
    int mu = mobius(k);
    return mu <= 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined; safety fallback */

    idx_t count = 0;
    for (idx_t k = 2;; ++k)           /* start from the first possible term */
    {
        if (included(k))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
