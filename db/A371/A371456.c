/*
 * A371456  Expansion of 1/(1 - x/(1 - 9*x^2)^(1/3)).
 * https://oeis.org/A371456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 9^k * binomial((n+k)/3-1, k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    idx_t maxk = n / 2;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = 1;
        idx_t mk = n + k;                    /* m + k   */
        for (idx_t t = 1; t <= k; ++t) {
            idx_t f = 3 * (mk - 3 - 3 * (t - 1));
            term = term * f;
            term = term / t;
        }
        sum += term;
    }
    return sum;
}
