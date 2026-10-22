/*
 * A381763  a(n) is the greatest k >= 0 such that Omega(n-i) = Omega(n+i) for
 *          1 <= i <= k, where Omega = A001222.
 * https://oeis.org/A381763
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max. k ≥ 0 such that Ω(n‑i) = Ω(n+i) for 1 ≤ i ≤ k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where Ω is the total number of prime factors counting multiplicity) */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Only defined for n ≥ 2 (offset 2). For smaller n, return 0. */
    if (n < 2)
        return 0;

    /* Search increasing i until the condition fails. */
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t om_left  = bigomega(n - i);   /* Ω(n‑i)  */
        seq_t om_right = bigomega(n + i);   /* Ω(n⁺i)  */
        if (om_left != om_right)
            return (seq_t)(i - 1);  /* largest k before failure */
    }

    /* If all pairs match up to i = n‑1, the maximum is n‑1. */
    return (seq_t)(n - 1);
}
