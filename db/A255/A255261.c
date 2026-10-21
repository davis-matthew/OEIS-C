/*
 * A255261  a(n) = a(n-1) + sum of the hexagonal numbers which are among the
 *          first (n-1) terms of the sequence, with a(1)=1.
 * https://oeis.org/A255261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + sum_{k < n-1, a(k) hexagonal} a(k) with a(1) =
 *            1. The function below computes the sequence iteratively, testing
 *            membership in the set of hexagonal numbers by checking whether
 *            8*x + 1 is a perfect square and (1 + sqrt(8*x+1)) is divisible
 *            by 4
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Check if a sequence value is a hexagonal number. */
static inline bool is_hexagonal(seq_t x)
{
    /* Compute t = 8*x + 1. */
    seq_t t = x * (seq_t)8 + (seq_t)1;
    /* Compute floor sqrt of t. */
    seq_t d = floor_sqrt(t);
    /* Verify that d*d == t (i.e., t is a perfect square). */
    if (d * d != t) return false;
    /* Check (1 + d) % 4 == 0. */
    return ((d + (seq_t)1) & (seq_t)3) == 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    seq_t a_val = (seq_t)1;          /* a(1) */
    seq_t sum_hex = (seq_t)1;        /* sum of hexagonal numbers among terms so far */

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        a_val += sum_hex;            /* a(i) = a(i‑1) + sum of previous hexagonal terms */

        /* If a(i) itself is hexagonal, add it to the running sum. */
        if (is_hexagonal(a_val))
            sum_hex += a_val;
    }

    return a_val;
}
