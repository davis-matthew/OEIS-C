/*
 * A337174  Number of pairs of divisors of n (d1,d2) such that d1 <= d2 and
 *          d1*d2 >= n.
 * https://oeis.org/A337174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((τ(n)+1)²/4), where τ(n) = number of divisors of n
 * Range:     u8 n <= 839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* defined only for n ≥ 1 in OEIS */

    /* Compute divisor count τ(n) by trial division */
    idx_t m = n;
    seq_t tau = 1;          /* τ(1) = 1 */

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            do {
                m /= p;
                ++exp;
            } while (m % p == 0);
            tau *= (seq_t)(exp + 1);
        }
    }
    if (m > 1) {
        /* Remaining prime factor with exponent 1 */
        tau *= 2;
    }

    /* a(n) = floor((tau+1)²/4) */
    seq_t t = tau + 1;
    return (t * t) / 4;
}
