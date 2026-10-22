/*
 * A389418  Least integer k such that Sum_{j=k..k+n-1} j is the concatenation
 *          of two consecutive integers, x and x - 1. No leading zeros
 *          allowed.
 * https://oeis.org/A389418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 87, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_concat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A389418:
   a(n) = least integer k such that
         Σ_{j=k}^{k+n-1} j = concat(x, x‑1)
   for some positive integer x (no leading zeros).  The sequence is
   1‑based. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_concat.h"

/* Find the least k such that the sum of n consecutive integers starting
   at k is a concatenation of two consecutive integers. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* fall‑back for out‑of‑range calls */

    /* The sum of k … k+n‑1 equals n*k + n*(n‑1)/2. */
    seq_t constBase = ((seq_t)n * ((seq_t)n - 1)) / 2;

    idx_t k = 1;
    while (1) {
        seq_t S = (seq_t)n * (seq_t)k + constBase;
        if (is_concat(S))
            return (seq_t)k;        /* cast to seq_t for consistency */
        ++k;
    }
}
