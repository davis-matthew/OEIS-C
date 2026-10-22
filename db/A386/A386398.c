/*
 * A386398  a(n) = Sum_{k=0..n} 5^(n-k) * binomial(n,k) * binomial(n+1,k).
 * https://oeis.org/A386398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} 5^(n-k) * binomial(n,k) * binomial(n+1,k)
 *
 * The implementation computes the sum directly using multiplicative
 * recurrence for the binomial coefficients and a powers-of‑5 array
 * that stays within the range of seq_t for all n that fit in the
 * available integer type.  It works for any seq_t that supports
 * integer arithmetic and for arbitrary‑precision builds.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* powers of 5: start with 5^n */
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow5 *= (seq_t)5;

    /* binomial(n, k) and binomial(n+1, k) iteratively */
    seq_t bin1 = 1;   /* C(n,0) */
    seq_t bin2 = 1;   /* C(n+1,0) */

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 5^(n-k) * C(n+1,k) * C(n,k) */
        seq_t term = pow5 * bin2;  /* 5^(n-k) * C(n+1,k) */
        term = term * bin1;        /* * C(n,k) */
        sum += term;

        if (k == n)
            break;

        /* update to next k */
        pow5 /= (seq_t)5;               /* 5^(n-(k+1)) */
        bin1 = (bin1 * (n - k)) / (k + 1);
        bin2 = (bin2 * (n + 1 - k)) / (k + 1);
    }

    return sum;
}
