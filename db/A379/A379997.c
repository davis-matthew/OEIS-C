/*
 * A379997  Expansion of e.g.f. 1/(exp(x) - x*exp(2*x))^2.
 * https://oeis.org/A379997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1) * (k-2)^(n-k)/(n-k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* fast unsigned exponentiation */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1U)
            result *= b;
        b *= b;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    seq_t P_prod = 1;         /* n!/(n-k)! for current k */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t exp = n - k;    /* exponent of (k-2) */
        int64_t base = (int64_t)k - 2;   /* may be negative */
        seq_t abs_base = base >= 0 ? (seq_t)base : (seq_t)(-base);

        /* compute |k-2|^exp */
        seq_t pow_abs = pow_uint(abs_base, exp);

        /* term = (k+1) * P_prod * |k-2|^exp  (unsigned magnitude) */
        seq_t term = (seq_t)(k + 1U) * P_prod;
        term *= pow_abs;

        /* sign of the term */
        bool negative = (base < 0) && (exp & 1U);

        if (negative)
            sum_neg += term;
        else
            sum_pos += term;

        /* update P_prod for next k: P_prod *= (n - k) */
        if (k != n) /* avoid multiplying by 0 when k==n */
            P_prod *= (seq_t)(n - k);
    }

    /* final result: positive difference */
    return sum_pos >= sum_neg ? sum_pos - sum_neg : 0;
}
