/*
 * A281433  Number of maximal matchings in the 2 X n rook graph.
 * https://oeis.org/A281433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (2*k-1)!!^2 * binomial(n,2*k) * (1
 *            + 2*k*(n-2*k))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t fac   = 1;  /* (2*0-1)!! = 1 */

    for (idx_t k = 0; k <= n / 2; ++k) {
        if (k > 0) {
            fac = fac * (seq_t)(2 * k - 1);   /* update (2*k-1)!! */
        }
        seq_t fac_sq = fac * fac;                            /* (2*k-1)!!^2 */
        seq_t bin    = binomial(n, 2 * k);                    /* binomial(n,2k) */
        seq_t t      = (seq_t)(1 + 2 * k * (n - 2 * k));      /* 1 + 2k(n-2k) */
        seq_t term   = fac_sq * bin;
        term = term * t;
        total = total + term;
    }
    return total;
}
