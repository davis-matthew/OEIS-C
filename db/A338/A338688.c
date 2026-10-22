/*
 * A338688  a(n) = - Sum_{d|n} (-n/d)^d * binomial(d+n/d-2, d-1).
 * https://oeis.org/A338688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Sum_{d|n} (-n/d)^d * binomial(d+n/d-2, d-1)
 * Range:     i8 n <= 8, i16 n <= 20, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses only integer arithmetic and the standard
 *  integer types.  The helper functions pow_seq and binom_seq are
 *  defined locally so no external utilities are required.   */

#include <stdint.h>

/* Power of a seq_t base to an integer exponent exp (non‑negative).  */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* Binomial coefficient C(n,k), computed multiplicatively.  */
/* Assumes 0 <= k <= n.  The intermediate result fits in seq_t
 * for all n that appear in the 64‑bit test range (the sequence grows
 * fast enough that terms that do not fit in seq_t are not checked). */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    if (k == 0)
        return 1;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (n-k+i) then divide by i.  The division is
         * exact because the binomial coefficient is integer. */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Main function computing A338688.  */
seq_t a(idx_t n)
{
    /* Sum over all divisors d of n.  */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;

        /* First divisor d1 = i. */
        idx_t d1 = i;
        idx_t m1 = n / d1;             /* m1 = n/d1 */
        seq_t base1 = -(seq_t)m1;
        seq_t pw1   = pow_seq(base1, d1);
        seq_t bin1  = binom_seq(d1 + m1 - 2, d1 - 1);
        sum += pw1 * bin1;

        /* Second divisor d2 = n/i, if distinct from d1. */
        idx_t d2 = m1;
        if (d2 != d1) {
            idx_t m2 = n / d2;             /* m2 = i */
            seq_t base2 = -(seq_t)m2;
            seq_t pw2   = pow_seq(base2, d2);
            seq_t bin2  = binom_seq(d2 + m2 - 2, d2 - 1);
            sum += pw2 * bin2;
        }
    }
    return -sum;
}
