/*
 * A349706  Square array T(n,k) = Sum_{j=0..k} binomial(k,j) * j^n for n and k
 *          >= 0, read by ascending antidiagonals.
 * https://oeis.org/A349706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = reading of the square array T(n,k) over antidiagonals,
 *            where T(n,k) = Σ_{j=0..k} binomial(k,j) * j^n
 * Range:     u8 n <= 38, u16 n <= 81, u32 n <= 177, u64 n <= 443,
 *            u128 n <= 1186, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    /* Adjust in case of rounding errors */
    idx_t tri = (idx_t)(((__uint128_t)s * (s + 1)) / 2);
    while (tri > n) {
        --s;
        tri = (idx_t)(((__uint128_t)s * (s + 1)) / 2);
    }
    while (((__uint128_t)(s + 1) * (s + 2)) / 2 <= n) {
        ++s;
        tri = (idx_t)(((__uint128_t)s * (s + 1)) / 2);
    }

    /* position within the diagonal */
    idx_t d = n - tri;
    idx_t n_val = s - d;   /* exponent for j^n_val */
    idx_t k_val = d;       /* upper limit for j */

    seq_t sum = 0;
    seq_t binom = 1; /* C(k_val,0) */
    for (idx_t j = 0; j <= k_val; ++j) {
        seq_t power = seq_pow((seq_t)j, n_val);
        sum += binom * power;
        if (j < k_val) {
            binom = binom * (seq_t)(k_val - j);
            binom = binom / (seq_t)(j + 1);
        }
    }
    return sum;
}
