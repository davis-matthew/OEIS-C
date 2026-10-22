/*
 * A371825  a(n) = Sum_{k=0..n} n^k * binomial(2*n,n-k).
 * https://oeis.org/A371825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} n^k * binomial(2*n, n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum  = 0;                    /* result */
    seq_t pow  = 1;                    /* n^k */
    /* initial binomial coefficient C(2n, n) */
    seq_t binom = binomial(2 * n, n);

    for (idx_t k = 0; k <= n; ++k) {
        /* add the k‑th term */
        sum += pow * binom;

        if (k == n)                 /* last iteration – finish */
            break;

        /* prepare for next k: n^(k+1) and C(2n, n-(k+1)) */
        pow *= n;
        binom = (binom * (n - k)) / (n + k + 1);
    }
    return sum;
}
