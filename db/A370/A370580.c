/*
 * A370580  a(n) = (n-1)! * Sum_{d|n} d/(d-1)!.
 * https://oeis.org/A370580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! * Σ_{d|n} d/(d-1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    if (n == 0) return 0;   /* not used (offset starts at 1) */

    seq_t res = 0;
    seq_t fact_n_minus_1 = factorial(n - 1);

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t fact_d1_minus_1 = factorial(d1 - 1);
            seq_t term1 = (fact_n_minus_1 / fact_d1_minus_1) * d1;
            res += term1;

            /* divisor d2 = n / i, unless equal to d1 */
            if (i != n / i) {
                idx_t d2 = n / i;
                seq_t fact_d2_minus_1 = factorial(d2 - 1);
                seq_t term2 = (fact_n_minus_1 / fact_d2_minus_1) * d2;
                res += term2;
            }
        }
    }
    return res;
}
