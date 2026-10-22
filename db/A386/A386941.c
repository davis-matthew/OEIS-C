/*
 * A386941  a(n) = Sum_{k=0..n} (2*k+1) * binomial(2*k,k) *
 *          binomial(2*n-k-1,n-k).
 * https://oeis.org/A386941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (2*k+1) * binomial(2*k,k) *
 *            binomial(2*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t ck = 1;                            /* C(0,0) */
    seq_t b = binomial(2 * n - 1, n);        /* B(0) = C(2n-1,n) */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = ck * (seq_t)(2 * k + 1);
        term = term * b;
        sum = sum + term;

        if (k == n)
            break;

        /* Update ck: C(2(k+1),(k+1)) = ck * ((2(k+1))*(2(k+1)-1)) / ((k+1)*(k+1)) */
        idx_t kp1 = k + 1;
        seq_t num = ck * (seq_t)(2 * kp1);
        num = num * (seq_t)(2 * kp1 - 1);
        seq_t den = (seq_t)(kp1 * kp1);
        ck = num / den;                       /* exact division */

        /* Update b: B(k+1) = B(k) * (n - k) / (2*n - k - 1) */
        seq_t numb = b * (seq_t)(n - k);
        seq_t denb = (seq_t)(2 * n - k - 1);
        b = numb / denb;                      /* exact division */
    }
    return sum;
}
