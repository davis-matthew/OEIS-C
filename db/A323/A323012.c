/*
 * A323012  a(n) = (1/sqrt(n^2+1)) * T_{2*n+1}(sqrt(n^2+1)) where T_{n}(x) is
 *          a Chebyshev polynomial of the first kind.
 * https://oeis.org/A323012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+1,2*k)*(n^2+1)^(n-k)*n^(2*k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t */
#include "../utils/binom.h"  /* provides static seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Pre‑compute constants n^2 and n^2+1 as seq_t. */
    seq_t n_sq  = (seq_t)n * (seq_t)n;
    seq_t n_sq1 = n_sq + (seq_t)1;    /* n^2 + 1 */

    /* Compute (n^2+1)^n once by repeated multiplication. */
    seq_t pow_nsq1_dec = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow_nsq1_dec *= n_sq1;

    /* Initialize for the iterative binomial recurrence. */
    idx_t   two_n_plus1 = 2*n + 1;
    idx_t   r           = 0;          /* current r in C(2n+1, r) with r even */
    seq_t   bin         = (seq_t)1;    /* binom(2n+1, 0) = 1 */
    seq_t   pow_n2k     = (seq_t)1;    /* n^(2k), starts with k=0 */

    seq_t  sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* term_k = binom * pow_n2k * pow_nsq1_dec */
        sum += bin * pow_n2k * pow_nsq1_dec;

        if (k == n)            /* last iteration: no need to update */
            break;

        /* compute next binom for r += 2 */
        /* bin = bin * (2n+1-r) / (r+1) * (2n-r) / (r+2) */
        seq_t tmp = bin * ((seq_t)two_n_plus1 - r);
        tmp /= (r + 1);                       /* division exact */
        tmp = tmp * ((seq_t)two_n_plus1 - r - 1);
        tmp /= (r + 2);                       /* division exact */
        bin = tmp;

        /* update powers for next k */
        pow_n2k *= n_sq;           /* n^(2(k+1)) = n^(2k) * n^2 */
        pow_nsq1_dec /= n_sq1;     /* (n^2+1)^(n-(k+1)) = (n^2+1)^(n-k) / (n^2+1) */

        r += 2;                    /* advance to next even r */
    }

    return sum;
}
