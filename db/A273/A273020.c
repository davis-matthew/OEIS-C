/*
 * A273020  a(n) = Sum_{k=0..n}
 *          C(n,k)*((-1)^n*(C(k,n-k)-C(k,n-k-1))+C(n-k,k+1)).
 * https://oeis.org/A273020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * ( (-1)^n * ( C(k,n-k) - C(k,n-k-1)
 *            ) + C(n-k,k+1) ) (offset 0) Implementation using only integer
 *            arithmetic and the provided seq_t type
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute binomial coefficient C(n,k) using multiplicative loop.
   Works for both small and large (seq_t) values. */
static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum_pos = 0;   /* accumulate positive contributions */
    seq_t sum_neg = 0;   /* accumulate negative contributions   */
    int sign_n = (n & 1) ? -1 : 1;          /* (-1)^n */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t Cnk = binom_small(n, k);
        if (Cnk == 0) continue;           /* only possible when n=0 and k>0 */

        /* Compute the difference part */
        seq_t a_val = binom_small(k, n - k);          /* C(k, n-k) */
        seq_t b_val = 0;                             /* C(k, n-k-1) */
        if (n > k)                                   /* only when n-k-1 >= 0 */
        {
            idx_t t2 = n - k - 1;
            b_val = binom_small(k, t2);
        }

        seq_t diff_abs = (a_val > b_val) ? (a_val - b_val) : (b_val - a_val);
        int diff_sign = 0;
        if (a_val > b_val) diff_sign = 1;
        else if (a_val < b_val) diff_sign = -1;

        int sign_term = sign_n * diff_sign;
        if (sign_term != 0 && diff_abs != 0)
        {
            seq_t prod = Cnk * diff_abs;
            if (sign_term > 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }

        /* Add the C(n-k, k+1) part */
        seq_t c2 = binom_small(n - k, k + 1);          /* C(n-k, k+1) */
        if (c2 != 0)
            sum_pos += Cnk * c2;
    }

    /* Final result: positive minus negative part */
    seq_t result = sum_pos;
    result -= sum_neg;
    return result;
}
