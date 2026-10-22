/*
 * A305007  Denominators of coefficients in expansion of Sum_{k>=1} x^k/(k*(1
 *          + x^k)).
 * https://oeis.org/A305007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{d|n} (-1)^(d+1)*d/n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n <= 0) {
        return 0;
    }

    long long S = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            long long sign1 = (d1 & 1) ? 1 : -1;
            S += sign1 * (long long)d1;

            if (d1 != d2) {
                long long sign2 = (d2 & 1) ? 1 : -1;
                S += sign2 * (long long)d2;
            }
        }
    }

    seq_t absS = (S >= 0) ? (seq_t)S : (seq_t)(-S);
    seq_t g = gcd((seq_t)n, absS);
    seq_t denom = ((seq_t)n) / g;
    return denom;
}
