/*
 * A291123  Number of ways to write 4*n+1 as (p-1)^2 + q^2 + r^2, where p is
 *          prime and q and r are nonnegative integers with q <= r.
 * https://oeis.org/A291123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 4056, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Count representations of 4*n+1 as (p-1)^2 + q^2 + r^2
   where p is prime, q,r are nonnegative integers, and q ≤ r.  */

/* Standard headers */
#include <stdbool.h>
#include <stdint.h>

/* Utility functions needed for this implementation */
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Target value to represent */
    seq_t T = (seq_t)4 * (seq_t)n + 1;

    /* Upper bound for p: (p-1)^2 ≤ T  ⇒  p ≤ sqrt(T) + 1 */
    idx_t p_limit = (idx_t)(floor_sqrt(T) + 1);

    seq_t total = 0;

    for (idx_t p = 2; p <= p_limit; ++p) {
        if (!is_prime(p))
            continue;

        /* Compute (p-1)^2 */
        seq_t p_minus_1 = (seq_t)p - 1;
        seq_t p2 = p_minus_1 * p_minus_1;

        /* Remaining amount to be expressed as q² + r² */
        seq_t S = T - p2;
        if (S < 0)
            continue; /* should never happen because of p_limit */

        /* Iterate q from 0 to sqrt(S) */
        idx_t q_max = (idx_t)floor_sqrt(S);
        for (idx_t q = 0; q <= q_max; ++q) {
            seq_t q2   = (seq_t)q * (seq_t)q;
            seq_t r2   = S - q2;
            seq_t r    = floor_sqrt(r2);
            if (r * r == r2 && (idx_t)r >= q) {
                ++total;
            }
        }
    }

    return total;
}
