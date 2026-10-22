/*
 * A332048  a(n) = n! * [x^n] 1 / (1 - LambertW(x))^n.
 * https://oeis.org/A332048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] 1 / (1 - LambertW(x))^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via integer arithmetic only. */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* B0 = C(2n-1, n-1) */
    seq_t B = 1;
    for (idx_t i = 1; i < n; ++i) {
        B = B * (seq_t)(n + i - 1) / (idx_t)i;
    }
    B = B * (seq_t)(2 * n - 1) / (idx_t)n;

    /* f_m = (n-1)! / m!  (start with m = 0) */
    seq_t f = factorial(n - 1);

    /* pow_abs = n^m (start with m = 0) */
    seq_t pow_abs = 1;

    seq_t sum_even = 0, sum_odd = 0;

    for (idx_t m = 0; m < n; ++m) {
        if (m > 0) {
            /* Update B, f, and pow_abs for next m */
            B = B * (seq_t)(n - m) / (seq_t)(2 * n - m);
            f = f / (idx_t)m;
            pow_abs = pow_abs * (seq_t)n;
        }

        /* term_abs = B * pow_abs * f (always positive) */
        seq_t term = B * pow_abs;
        term = term * f;

        if ((m & 1) == 0) {
            sum_even += term;
        } else {
            sum_odd += term;
        }
    }

    seq_t diff = sum_even - sum_odd;   /* even sum >= odd sum for all n ≥ 0 */
    return diff * (seq_t)n;
}
