/*
 * A322243  a(n) = A322242(n)^2, the square of the central coefficient in (1 +
 *          3*x + 4x^2)^n.
 * https://oeis.org/A322243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A322242(n)^2 , where A322242(n) = Sum_{k=0..n} (-1)^(n-k)
 *            * 2^k * binomial(n,k) * binomial(2k,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   // for typedefs used by the build system

seq_t a(idx_t n)
{
    seq_t pos = 0;            // sum of positive terms
    seq_t neg = 0;            // sum of negative terms
    seq_t binom_n_k = 1;      // binomial(n,0)
    seq_t binom_2k_k = 1;     // binomial(0,0)
    seq_t pow2 = 1;           // 2^k

    for (idx_t k = 0; k <= n; ++k) {
        /* current term: 2^k * binom(n,k) * binom(2k,k) */
        seq_t cur = binom_n_k * binom_2k_k * pow2;

        /* sign = (-1)^(n-k) */
        if ( ((n - k) & 1U) == 0 )  /* even -> positive */
            pos += cur;
        else                        /* odd -> negative */
            neg += cur;

        if (k == n) break;         /* last iteration finished */

        /* update binomial(n,k+1) = binom(n,k) * (n-k)/(k+1) */
        binom_n_k *= (seq_t)(n - k);
        binom_n_k /= (seq_t)(k + 1);

        /* update binomial(2(k+1),(k+1))
           = binom(2k,k) * ((2k+2)(2k+1))/((k+1)^2) */
        seq_t num   = (seq_t)(2*k + 2) * (seq_t)(2*k + 1);
        seq_t denom = (seq_t)(k + 1) * (seq_t)(k + 1);
        binom_2k_k *= num;
        binom_2k_k /= denom;

        pow2 *= 2;                 /* 2^(k+1) */
    }

    /* compute the central coefficient: pos - neg */
    seq_t central = pos - neg;

    /* return its square */
    return central * central;
}
