/*
 * A280723  a(n) is the denominator of 6 * Sum_{k=0..n} ((k+1)/(n-k+1)^2) *
 *          (Catalan(k)/(2^(2*k+1)))^2.
 * https://oeis.org/A280723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the denominator of 6 * sum_{k=0..n} ((k+1)/(n-k+1)^2) *
 *            * * (Catalan(k)/(2^(2*k+1)))^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   catalan, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow2.h"
#include "../utils/catalan.h"

seq_t a(idx_t n) {
    /* rational sum: sum_num / sum_den */
    seq_t sum_num = 0;          /* numerator of the sum */
    seq_t sum_den = 1;          /* denominator of the sum */
    seq_t C = (seq_t)1;         /* Catalan(0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term k: 6*(k+1)*C_k^2 / ((n-k+1)^2 * 2^(4*k+2)) */
        seq_t k_plus1 = (seq_t)k + 1;
        seq_t Csq = C * C;
        seq_t term_num = Csq * k_plus1;
        term_num = term_num * 6;                /* multiply by 6 */

        seq_t part = (seq_t)(n - k + 1);
        seq_t denom_part = part * part;          /* (n-k+1)^2 */
        seq_t term_pow  = pow2(4 * k + 2);      /* 2^(4*k+2) */
        seq_t term_den  = denom_part * term_pow;

        /* add term_num/term_den to the current sum */
        seq_t g = gcd(sum_den, term_den);
        seq_t sum_den_div_g = sum_den / g;
        seq_t term_den_div_g = term_den / g;
        seq_t new_num = sum_num * term_den_div_g + term_num * sum_den_div_g;
        seq_t lcm = sum_den_div_g * term_den;   /* lcm of denominators */
        seq_t g2 = gcd(new_num, lcm);
        sum_num = new_num / g2;
        sum_den = lcm    / g2;

        /* compute next Catalan number for k+1 */
        if (k < n) {
            seq_t two = (seq_t)2;
            seq_t numerator = C * ( two * ((seq_t)2 * k + 1) );
            seq_t denominator = (seq_t)k + 2;
            C = numerator / denominator;          /* exact division */
        }
    }

    return sum_den;        /* denominator of the required sum */
}
