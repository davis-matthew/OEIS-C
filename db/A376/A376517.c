/*
 * A376517  E.g.f. satisfies A(x) = exp(x^2 * (1 + x) * A(x)).
 * https://oeis.org/A376517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376517: a(n) = n! * Σ_{k=0..⌊n/2⌋} (k+1)^(k-1) * binomial(k, n-2k) / k! */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Compute the sequence value at index n. */
seq_t a(idx_t n)
{
    /* Special case n = 0, though the formula already handles it. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* n! */
    seq_t fac_n = factorial(n);

    /* Terms are n! / k! * (k+1)^(k-1) * binomial(k, n-2k). */
    seq_t sum = (seq_t)0;

    /* Current ratio = n! / k! .  Start with k = 0. */
    seq_t ratio = fac_n;

    for (idx_t k = (idx_t)0; k <= n >> 1; ++k) {
        /* m = n - 2k */
        idx_t m = n - (k << 1);
        if (m > k) {
            /* binomial(k, m) = 0 when m > k, skip. */
        } else {
            seq_t bin = binomial(k, m);  /* C(k, m) */
            if (bin != (seq_t)0) {
                /* (k+1)^(k-1)  (k=0 term contributes 1 if m==0, but bin=1 only for n=0) */
                seq_t powk;
                if (k == (idx_t)0) {
                    powk = (seq_t)1;
                } else {
                    powk = ipow((seq_t)(k + 1), (seq_t)(k - 1));
                }

                /* term = ratio * powk * bin.  Ratio already equals n! / k!. */
                seq_t term = ratio * powk;
                term = term * bin;
                sum += term;
            }
        }

        /* Prepare ratio for next k: divide by (k+1) to get n! / (k+1)! . */
        if (k + (idx_t)1 <= n)
            ratio /= (seq_t)(k + 1);
    }

    return sum;
}
