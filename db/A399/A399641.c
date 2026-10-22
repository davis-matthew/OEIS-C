/*
 * A399641  Expansion of g^5/(3-2*g)^4, where g = 1+x*g^3 is the g.f. of
 *          A001764.
 * https://oeis.org/A399641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)/2)*(binomial(3*n+2,n) + Σ_{k=0..n}
 *            2^(n-k)*binomial(3*n+3,k))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Compute binomial(3*n+2, n) */
    seq_t term = binom(3*n + 2, n);

    /* Sum Σ 2^(n-k)*binomial(3*n+3,k) using incremental formulas */
    seq_t sum2 = 0;
    seq_t power = pow2(n);           /* 2^n */
    seq_t binom_k = 1;               /* binomial(3*n+3, 0) */
    idx_t maxk = n;

    for (idx_t k = 0; k <= maxk; ++k) {
        if (k > 0) {
            /* binom(3*n+3, k) = binom(3*n+3, k-1) * (3*n+4 - k) / k */
            binom_k = (binom_k * (seq_t)(3*n + 4 - k)) / (seq_t)k;
        }
        sum2 += power * binom_k;
        power /= 2;
    }

    term += sum2;

    /* Multiply by (n+1)/2, avoiding overflow by dividing first if possible */
    idx_t np1 = n + 1;
    seq_t result;
    if ((np1 & 1) == 0) {          /* n+1 even */
        result = ((seq_t)(np1 / 2)) * term;
    } else {                        /* n+1 odd, term must be even */
        result = (term / 2) * (seq_t)np1;
    }
    return result;
}
