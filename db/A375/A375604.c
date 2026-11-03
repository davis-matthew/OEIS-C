/*
 * A375604  Expansion of e.g.f. 1 / (exp(-x^2) - x).
 * https://oeis.org/A375604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (n-2k+1)^k / k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n) {
    /* Compute n! */
    seq_t prod_cur = factorial(n);

    seq_t sum = 0;
    idx_t maxk = n / 2;          /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Base for the power term */
        seq_t base = (seq_t)(n - 2 * k + 1);

        /* (n-2k+1)^k */
        seq_t powk = ipow(base, (seq_t)k);

        /* Term = prod_cur * powk */
        seq_t term = prod_cur * powk;
        sum += term;

        /* Update prod_cur for next k: divide by (k+1) */
        if (k < maxk) {
            prod_cur /= (k + 1);
        }
    }

    return sum;
}
