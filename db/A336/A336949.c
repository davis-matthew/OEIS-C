/*
 * A336949  a(n) = n! * [x^n] 1 / (exp(-n*x) - x).
 * https://oeis.org/A336949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n*(n-k+1))^k / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 191 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* n! */
    seq_t fact_n = factorial(n);
    /* factor == n! / k! for the current value of k */
    seq_t factor = fact_n;
    seq_t total  = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t pow_val;
        if (k == 0)
        {
            pow_val = 1;
        }
        else
        {
            seq_t base = (seq_t)n * ((seq_t)n - (seq_t)k + 1);
            pow_val = ipow(base, (seq_t)k);
        }

        seq_t term = pow_val * factor;
        total += term;

        if (k < n)
        {
            factor /= (seq_t)(k + 1);
        }
    }

    return total;
}
