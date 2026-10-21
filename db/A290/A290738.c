/*
 * A290738  a(n) is the number of fixed polycubes of size n that are proper in
 *          n-5 dimensions.
 * https://oeis.org/A290738
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-12)*n^(n-11)*(n-5)*(240*n^11 - 6000*n^10 + 62240*n^9
 *            * - 356232*n^8 + 1335320*n^7 - 4062240*n^6 + 12397445*n^5 * -
 *            42322743*n^4 + 150403080*n^3 - 535510740*n^2 + 1923269040*n * -
 *            3731495040)/45
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the number of fixed polycubes of size n that are proper
 * in n-5 dimensions.  Offset: 5. */
/* Includes: <stdint.h>, "../utils/pow_seq.h", "../utils/gcd.h" */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* For n < 5 the value is undefined; return 0. */
    if (n < 5) return 0;

    /* Convert the index to seq_t once. */
    seq_t nval = (seq_t)n;

    /* Coefficients of the polynomial in decreasing degree. */
    const long long coeff[12] = {
        240LL,
        -6000LL,
        62240LL,
        -356232LL,
        1335320LL,
        -4062240LL,
        12397445LL,
        -42322743LL,
        150403080LL,
        -535510740LL,
        1923269040LL,
        -3731495040LL
    };

    /* Evaluate the 11th-degree polynomial using Horner's rule. */
    seq_t poly = (seq_t)coeff[0];
    for (int i = 1; i < 12; ++i) {
        poly = poly * nval;
        if (coeff[i] >= 0) {
            poly = poly + (seq_t)coeff[i];
        } else {
            poly = poly - (seq_t)(-coeff[i]);
        }
    }

    /* Handle small n (5 <= n < 12) by computing numerator / denominator
     * directly, where certain powers are negative. */
    if (n < 12) {
        /* Numerator: (n-5)*poly (no extra powers). */
        seq_t numerator = (seq_t)(n - 5) * poly;

        /* Denominator: 45 * 2^(12-n) * n^(11-n). */
        seq_t denom = (seq_t)45;
        denom = denom * pow_seq((seq_t)2, (int)(12 - n));
        denom = denom * pow_seq(nval, (int)(11 - n));

        return numerator / denom;
    }

    /* For n >= 12 we cancel the denominator 45 as early as possible
     * to keep intermediate values small. */
    seq_t res = 1;
    seq_t den = (seq_t)45;

    /* Factor (n-5). */
    seq_t factor = (seq_t)(n - 5);
    seq_t g = gcd(factor, den);
    res = res * (factor / g);
    den  = den / g;

    /* Factor the polynomial. */
    g = gcd(poly, den);
    res = res * (poly / g);
    den = den / g;

    /* Factor 2^(n-12). */
    seq_t pow2 = pow_seq((seq_t)2, (int)(n - 12));
    g = gcd(pow2, den);
    res = res * (pow2 / g);
    den = den / g;

    /* Factor n^(n-11). */
    seq_t pown = pow_seq(nval, (int)(n - 11));
    g = gcd(pown, den);
    res = res * (pown / g);
    den = den / g;

    /* At this point, den should be 1. */
    return res;
}
