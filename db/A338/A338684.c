/*
 * A338684  a(n) = Sum_{d|n} (-1)^(d-1) * (n/d)^n * binomial(d+n/d-1, d).
 * https://oeis.org/A338684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * (n/d)^n * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial, compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Important: the function depends only on the standard integer types and
 * three provided utilities: big_pow, binomial, and compare_seq_t. */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/compare_seq_t.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety, we handle n==0
     * (even though the offset is 1) by returning zero. */
    if (n == 0)
        return (seq_t)0;

    seq_t pos = (seq_t)0;   /* sum of terms with odd divisor d (positive sign) */
    seq_t neg = (seq_t)0;   /* sum of terms with even divisor d (negative sign) */

    /* Enumerate all positive divisors d of n; for each, m = n/d. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* First divisor: d */
        idx_t m  = n / d;
        seq_t pow_m = big_pow((seq_t)m, (seq_t)n);
        seq_t bin    = binomial(d + m - 1, d);
        seq_t term   = pow_m * bin;

        if (d & 1U)   /* d is odd -> positive sign */
            pos += term;
        else
            neg += term;

        /* Second divisor: d2 = n/d, only if distinct from d */
        idx_t d2 = n / d;
        if (d2 != d) {
            idx_t m2  = n / d2;   /* m2 == d */
            seq_t pow_m2 = big_pow((seq_t)m2, (seq_t)n);
            seq_t bin2    = binomial(d2 + m2 - 1, d2);
            seq_t term2   = pow_m2 * bin2;

            if (d2 & 1U)
                pos += term2;
            else
                neg += term2;
        }
    }

    /* The final result is pos - neg.  The sequence is known to be
     * non‑negative; however, we compute a safe difference in case of a
     * mis‑specification. */
    if (compare_seq_t((const void *)&pos, (const void *)&neg) >= 0)
        return pos - neg;
    else
        return neg - pos;   /* should not happen for the sequence */
}
