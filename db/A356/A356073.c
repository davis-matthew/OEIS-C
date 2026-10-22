/*
 * A356073  Number of labeled trees on [n] having a centroid.
 * https://oeis.org/A356073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2) if n is odd, a(n) = n^(n-2) -
 *            C(n,n/2)*(n/2)^(n-2)/2 if n is even. a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* For odd n, simply n^(n-2).  For even n, subtract the adjustment. */
    if (n & 1) {
        return big_pow((seq_t)n, (seq_t)(n - 2));
    } else {
        seq_t pow_total = big_pow((seq_t)n, (seq_t)(n - 2));

        seq_t bin_coeff  = binomial(n, n / 2);
        seq_t pow_half   = big_pow((seq_t)(n / 2), (seq_t)(n - 2));

        /* Compute sub = bin_coeff * pow_half / 2 safely to avoid overflow. */
        seq_t sub = seq_mul(bin_coeff, pow_half);
        sub = sub / 2;   /* division by 2 */

        return pow_total - sub;
    }
}
