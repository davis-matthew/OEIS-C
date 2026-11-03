/*
 * A376477  E.g.f. satisfies A(x) = exp( x^3*A(x)^3 * (1 + x*A(x)) ).
 * https://oeis.org/A376477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} (n+1)^(k‑1) * binomial(k,n‑3k) / k!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

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

seq_t a(idx_t n) {
    /* Fast return for the first term. */
    if (n == 0) {
        return 1;
    }

    /* Compute n! once, it is needed for every k. */
    seq_t fac_n = factorial(n);
    seq_t result = 0;

    /* Loop over all admissible k. */
    for (idx_t k = 0; k <= n / 3; ++k) {
        idx_t m = n - 3 * k;          /* m = n - 3k  */
        if (m > k) {                  /* binomial(k,m) = 0 for m > k */
            continue;
        }

        /* Special handling of k = 0 (only contributes when n = 0). */
        if (k == 0) {
            /* For n > 0, binomial(k,m) == 0, nothing to add. */
            continue;
        }

        /* Compute individual components of the summand. */
        seq_t bin = binomial(k, m);               /* binomial(k, m) */
        seq_t pow = ipow_i32((seq_t)(n + 1), (int)(k - 1)); /* (n+1)^(k-1) */

        /* n! / k! */
        seq_t fac_k = factorial(k);
        seq_t pref = fac_n / fac_k;               /* exact division */

        /* Assemble the term: (n! / k!) * (n+1)^(k-1) * binomial(k,m) */
        seq_t term = pref;
        term *= pow;
        term *= bin;

        result += term;
    }

    return result;
}
