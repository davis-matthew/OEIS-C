/*
 * A307811  Expansion of 1/AGM(1-45*x, sqrt((1-25*x)*(1-81*x))).
 * https://oeis.org/A307811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{k=0..n} 5^(n-k) * binomial(n,k) * binomial(2k,k))^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t pow5 = seq_pow((seq_t)5, n);

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin_n_k = binomial(n, k);          /* C(n,k)  */
        seq_t bin_2k_k = binomial(2 * k, k);     /* C(2k,k) */
        seq_t term = bin_n_k;
        term *= bin_2k_k;
        term *= pow5;
        sum += term;

        if (k == n) break;          /* avoid reducing pow5 after last term */
        pow5 /= (seq_t)5;           /* prepare 5^(n-(k+1)) for next loop */
    }

    return sum * sum;
}
