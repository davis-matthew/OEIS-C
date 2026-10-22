/*
 * A365609  G.f. satisfies A(x) = 1 + x^2*A(x)^4*(1 + x*A(x)).
 * https://oeis.org/A365609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(k,n-2*k) *
 *            binomial(n+2*k+1,k) / (n+2*k+1) which can be rewritten as a(n) =
 *            sum_{k=ceil(n/3)..floor(n/2)} binomial(k,n-2*k) *
 *            binomial(n+2*k, k-1) / k This implementation computes the terms
 *            directly using the derived recurrence and avoids repeated
 *            factorial o...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Determine the range of k to sum over */
    idx_t k_start = (n + 2) / 3;   /* ceil(n/3) */
    idx_t k_end   =  n / 2;        /* floor(n/2) */
    if (k_start > k_end) return (seq_t)0;

    seq_t sum = (seq_t)0;

    /* Initial binomial values for the first term */
    seq_t f = binom(k_start, n - 2 * k_start);                /* binom(k, n-2k)   */
    seq_t H = binom(n + 2 * k_start, k_start - 1);           /* binom(n+2k, k-1) */

    for (idx_t k = k_start; k <= k_end; ++k)
    {
        /* term = f * H / k */
        seq_t term = (f * H) / (seq_t)k;
        sum += term;

        if (k == k_end) break;   /* no need to update after last term */

        /* Update f to f_{k+1} */
        seq_t r_prev = (seq_t)(n - 2 * k);          /* current n-2k */
        seq_t tmp_f  = f * (seq_t)(k + 1);
        tmp_f        = tmp_f * r_prev;
        tmp_f        = tmp_f * (r_prev - (seq_t)1);
        seq_t denom_f = ((seq_t)(k - r_prev + 3) *
                         (k - r_prev + 2) *
                         (k - r_prev + 1));
        f = tmp_f / denom_f;                       /* exact division */

        /* Update H to H_{k+1} */
        seq_t m      = (seq_t)n + (seq_t)2 * k;   /* m = n + 2k          */
        seq_t num_H  = (m + 2) * (m + 1);         /* (m+2)(m+1)          */
        seq_t den_H  = (seq_t)k * (m - (seq_t)k + 2); /* k*(m-k+2)           */
        H = (H * num_H) / den_H;                 /* exact division */
    }

    return sum;
}
