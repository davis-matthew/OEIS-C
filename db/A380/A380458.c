/*
 * A380458  Characteristic function of A337533 (union of {1} with nonsquares
 *          whose square part's square root is in the sequence).
 * https://oeis.org/A380458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is in the set defined by A337533 (union of {1}
 *            with nonsquares whose square part's square root is in the set),
 *            otherwise 0
 * Range:     u8 n <= 63, u16 n <= 9722, u32 n <= 9722, u64 n <= 9722,
 *            u128 n <= 9722, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Test whether n belongs to the set A337533.  The test is
   recursive: n ∈ A337533 ⇔
      n == 1                                              – true
      n is a perfect square                                 – false
      otherwise let m = sqrt(square part of n);
      then n ∈ A337533 ⇔ m ∈ A337533.  Since m < n, the
      recursion terminates.                                 */
static bool in_A337533(idx_t n)
{
    /* Base case */
    if (n == 1)
        return true;

    /* Test for perfect square – avoid overflow by using idx_t */
    idx_t sqrt_n = (idx_t)floor_sqrt((seq_t)n);
    if (sqrt_n * sqrt_n == n)
        return false;

    /* Compute m = sqrt(square part of n) */
    idx_t temp = n;
    idx_t m = 1;

    /* Handle factor 2 specially */
    idx_t cnt = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++cnt;
    }
    if (cnt >= 2) {
        for (idx_t i = 0; i < cnt/2; ++i)
            m *= 2;
    }

    /* Handle odd prime factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            if (cnt >= 2) {
                for (idx_t i = 0; i < cnt/2; ++i)
                    m *= p;
            }
        }
    }
    /* Any remaining prime factor has exponent 1 and does not affect m */

    /* Recursively test m.  If m == 1, n is square‑free and
       therefore in the set. */
    if (m == 1)
        return true;

    return in_A337533(m);
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    return (seq_t)(in_A337533(n) ? 1 : 0);
}
