/*
 * A290723  a(n) = (1/11520) * n*(n+7)^2 * (3*n^7 + 83*n^6 + 961*n^5 +
 *          6201*n^4 + 24708*n^3 + 60700*n^2 + 87968*n + 85056).
 * https://oeis.org/A290723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/11520) * n*(n+7)^2 * (3*n^7 + 83*n^6 + 961*n^5 +
 *            6201*n^4 + 24708*n^3 + 60700*n^2 + 87968*n + 85056)
 * Range:     u8 none, u16 n <= 0, u32 n <= 16, u64 n <= 188, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t nn = n;
    seq_t denom = (seq_t)11520;

    /* compute the four multiplicative factors */
    seq_t f0 = nn;                      /* n */
    seq_t f1 = nn + (seq_t)7;           /* n+7 */
    seq_t f2 = f1;                      /* (n+7) again */

    /* compute the polynomial by Horner's rule */
    seq_t poly = (seq_t)3;
    poly = poly * nn + (seq_t)83;
    poly = poly * nn + (seq_t)961;
    poly = poly * nn + (seq_t)6201;
    poly = poly * nn + (seq_t)24708;
    poly = poly * nn + (seq_t)60700;
    poly = poly * nn + (seq_t)87968;
    poly = poly * nn + (seq_t)85056;
    seq_t f3 = poly;

    /* bundle the factors for easier handling */
    seq_t factors[4] = {f0, f1, f2, f3};

    /* Reduce the denominator by gcds with each factor */
    for (idx_t i = 0; i < 4; ++i) {
        if (denom == (seq_t)1) break;
        seq_t g = gcd(factors[i], denom);
        if (g != (seq_t)1) {
            factors[i] /= g;
            denom /= g;
        }
    }

    /* Multiplying the reduced factors gives the exact result. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < 4; ++i) {
        result *= factors[i];
    }

    /* Just in case the denominator was not fully cancelled, finish the division. */
    if (denom != (seq_t)1) {
        result /= denom;
    }

    return result;
}
