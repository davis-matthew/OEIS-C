/*
 * A251590  a(n) = 10^(n-8) * (n+1)^(n-10) * (4782969*n^8 + 50309748*n^7 +
 *          237013938*n^6 + 655232760*n^5 + 1166624361*n^4 + 1374998212*n^3 +
 *          1051760172*n^2 + 479277840*n + 100000000).
 * https://oeis.org/A251590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^(n-8) * (n+1)^(n-10) * (4782969*n^8 + 50309748*n^7 +
 *            237013938*n^6 + 655232760*n^5 + 1166624361*n^4 + 1374998212*n^3
 *            + 1051760172*n^2 + 479277840*n + 100000000)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   pow10, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 10^(n-8) and (n+1)^(n‑10) with possible denominators for n<8 or n<10 */
    seq_t pow10_num, pow10_den, pow_n1_num, pow_n1_den;

    if (n >= 8) {
        pow10_num = pow10(n - 8);
        pow10_den = (seq_t)1;
    } else {
        pow10_num = (seq_t)1;
        pow10_den = pow10(8 - n);
    }

    seq_t base_n1 = (seq_t)(n + 1);
    if (n >= 10) {
        pow_n1_num = seq_pow(base_n1, n - 10);
        pow_n1_den = (seq_t)1;
    } else {
        pow_n1_num = (seq_t)1;
        pow_n1_den = seq_pow(base_n1, 10 - n);
    }

    /* Polynomial P(n) evaluated with Horner's method */
    static const seq_t coeff[9] = {
        (seq_t)4782969,
        (seq_t)50309748,
        (seq_t)237013938,
        (seq_t)655232760,
        (seq_t)1166624361,
        (seq_t)1374998212,
        (seq_t)1051760172,
        (seq_t)479277840,
        (seq_t)100000000
    };

    seq_t n_seq = (seq_t)n;
    seq_t poly = coeff[0];
    for (idx_t i = 1; i < 9; ++i) {
        poly = poly * n_seq + coeff[i];
    }

    /* Assemble numerator and denominator, then divide (exact division) */
    seq_t numerator   = pow10_num * pow_n1_num * poly;
    seq_t denominator = pow10_den * pow_n1_den;

    return numerator / denominator;
}
