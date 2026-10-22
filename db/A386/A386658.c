/*
 * A386658  E.g.f.: Sum_{n>=0} (5^n*x + LambertW(x))^n / n!.
 * https://oeis.org/A386658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k) * 5^(n*k) * (1 -
 *            (n-k)/5^k)^(n-k-1) (derived from the OEIS definition)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* term for k = n : 5^(n^2) */
    seq_t sum_pos = seq_pow((seq_t)5, (idx_t)((seq_t)n * n));
    if (n == 0)
        return sum_pos;

    seq_t p5      = (seq_t)5;
    seq_t pow5_n  = seq_pow(p5, n);           /* 5^n  */
    seq_t d       = pow5_n / p5;              /* 5^(n-1)  */
    seq_t binom   = (seq_t)n;                 /* binom(n,n-1) */
    seq_t sum_neg = 0;

    for (idx_t k = n - 1; ; --k)
    {
        /* factor = 5^(k*(k+1)) = (5^k)^(k+1) = d^(k+1) */
        seq_t factor = seq_pow(d, k + 1);

        seq_t n_minus_k = (seq_t)(n - k);
        int   neg_base  = d < n_minus_k;           /* does 5^k < (n-k)? */
        seq_t abs_base  = neg_base ? n_minus_k - d : d - n_minus_k;

        idx_t e = n - k - 1;                      /* exponent of base */
        seq_t base_pow = seq_pow(abs_base, e);

        seq_t term_mag = binom * factor * base_pow;

        /* sign of the term: negative only when base was negative
           and e is odd */
        if (neg_base && (e & 1))
            sum_neg += term_mag;
        else
            sum_pos += term_mag;

        if (k == 0)
            break;

        /* prepare values for next k (k -> k-1) */
        d = d / p5;  /* 5^(k-1) */

        /* binom(n,k-1) = binom(n,k) * k / (n-k+1) */
        seq_t tmp = binom * (seq_t)k;
        seq_t denom = (seq_t)(n - k + 1);
        binom = tmp / denom;
    }

    return sum_pos - sum_neg;
}
