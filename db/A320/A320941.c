/*
 * A320941  Expansion of Sum_{k>=1} x^k*(1 + x^k)/(1 - x^k)^4.
 * https://oeis.org/A320941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d*(d+1)*(2*d+1)/6
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 2219, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t divisor_term(idx_t d) {
    /* use wider type to avoid overflow */
    unsigned __int128 x = (unsigned __int128)d *
                         ( (unsigned __int128)d + 1 ) *
                         ( (unsigned __int128)2 * d + 1 );
    return (seq_t)(x / 6);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined below offset, return 0 */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += divisor_term(d);
            idx_t e = n / d;
            if (e != d) {
                sum += divisor_term(e);
            }
        }
    }
    return sum;
}
