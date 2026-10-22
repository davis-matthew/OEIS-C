/*
 * A378378  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+3*k-1,3*k).
 * https://oeis.org/A378378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n,k) * binomial(n+3*k-1,3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes the n-th term of OEIS A378378.  */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom_nk = 1;   /* binomial(n,0) */
    seq_t Bk       = 1;   /* binomial(n-1,0) = 1 */

    /* Loop over k = 0 .. n */
    for (idx_t k = 0; k <= n; ++k) {
        sum += binom_nk * Bk;

        if (k == n) break;          /* no more terms */

        /* Update binomial(n,k+1) from binomial(n,k)  */
        binom_nk = ( binom_nk * (seq_t)(n - k) ) / (seq_t)(k + 1);

        /* Update B_{k+1} from B_k.
           B_{k+1} = B_k * (n+3k+2)(n+3k+1)(n+3k) / [(3k+3)(3k+2)(3k+1)] */
        seq_t numer[3];
        seq_t denom[3];

        numer[0] = (seq_t)(n + 3*k + 2);
        numer[1] = (seq_t)(n + 3*k + 1);
        numer[2] = (seq_t)(n + 3*k);

        denom[0] = (seq_t)(3*k + 3);
        denom[1] = (seq_t)(3*k + 2);
        denom[2] = (seq_t)(3*k + 1);

        seq_t val = Bk;

        for (int i = 0; i < 3; ++i) {
            /* Reduce numerator factor with pending denominators */
            for (int j = 0; j < 3; ++j) {
                seq_t g = gcd(numer[i], denom[j]);
                if (g > (seq_t)1) {
                    numer[i] /= g;
                    denom[j] /= g;
                }
            }
            /* Multiply the reduced numerator factor */
            val *= numer[i];

            /* Reduce val with remaining denominators */
            for (int j = 0; j < 3; ++j) {
                seq_t g = gcd(val, denom[j]);
                if (g > (seq_t)1) {
                    val /= g;
                    denom[j] /= g;
                }
            }
        }

        /* At this point all remaining denominators should be 1. */
        Bk = val;
    }

    return sum;
}
