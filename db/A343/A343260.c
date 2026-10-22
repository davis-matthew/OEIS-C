/*
 * A343260  a(n) = 2 * T(n,(n+1)/2) where T(n,x) is a Chebyshev polynomial of
 *          the first kind.
 * https://oeis.org/A343260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*∑_{k=0}^{n} (n-1)^k * binomial(n+k,2k)/(n+k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)2;

    seq_t sum_num = 0;          /* numerator of the running sum */
    seq_t sum_den = 1;          /* denominator of the running sum */
    seq_t pow = 1;              /* (n-1)^k, start with k=0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n + k, 2 * k);         /* C(n+k,2k) */
        seq_t term_num = pow * bin;                 /* (n-1)^k * C(n+k,2k) */
        seq_t term_den = (seq_t)(n + k);             /* division by (n+k) later */

        /* Add term_num/term_den to the running sum:  sum += term */
        seq_t g1 = seq_gcd(sum_den, term_den);
        seq_t lcm_factor1 = sum_den / g1;           /* sum_den / g1 */
        seq_t lcm_factor2 = term_den / g1;          /* term_den / g1 */

        seq_t new_num = sum_num * lcm_factor2 + term_num * lcm_factor1;
        seq_t new_den = lcm_factor1 * term_den;

        seq_t g2 = seq_gcd(new_num, new_den);
        sum_num = new_num / g2;
        sum_den = new_den / g2;

        /* Prepare power for next iteration */
        if (k < n)
            pow = pow * (seq_t)(n - 1);
    }

    /* a(n) = 2 * n * (sum_num / sum_den) = (2*n*sum_num) / sum_den */
    seq_t tmp = 2 * (seq_t)n * sum_num;
    return tmp / sum_den;
}
