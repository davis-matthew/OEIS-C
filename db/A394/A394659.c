/*
 * A394659  Number of independent vertex sets of size 7 in the n-triangular
 *          grid graph.
 * https://oeis.org/A394659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 for n ≤ 4 a(n) = (n-4) * P(n) / 645120 for n > 4 where
 *            P(n) = n^13 + 25·n^12 + 9·n^11 - 3555·n^10 - 7745·n^9 +
 *            260563·n^8 + 291075·n^7 - 11898033·n^6 + 10339924·n^5 +
 *            303368632·n^4 - 909281184·n^3 - 2609966352·n^2 + 16737321600·n -
 *            23213352960
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 4) return 0;

    /* coefficients of P(n), index 0 corresponds to n^13 */
    static const long long coeff[14] = {
        1LL,          /* n^13 */
        25LL,         /* n^12 */
        9LL,          /* n^11 */
        -3555LL,      /* n^10 */
        -7745LL,      /* n^9  */
        260563LL,     /* n^8  */
        291075LL,     /* n^7  */
        -11898033LL,  /* n^6  */
        10339924LL,   /* n^5  */
        303368632LL,  /* n^4  */
        -909281184LL, /* n^3  */
        -2609966352LL,/* n^2  */
        16737321600LL,/* n^1  */
        -23213352960LL/* n^0  */
    };

    seq_t nn = (seq_t)n;

    /* pre‑compute powers of n: pow[e] = n^e for e = 0..13 */
    seq_t pow[14];
    pow[0] = (seq_t)1;
    for (int e = 1; e <= 13; ++e)
        pow[e] = pow[e-1] * nn;

    /* Separate positive and negative contributions to avoid
       temporary signed values in an unsigned type. */
    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    for (int idx = 0; idx < 14; ++idx) {
        int exp = 13 - idx;                /* exponent for this term */
        seq_t term = pow[exp];
        if (coeff[idx] >= 0) {
            pos += term * (seq_t)coeff[idx];
        } else {
            neg += term * (seq_t)(-coeff[idx]);
        }
    }

    seq_t diff = pos - neg;               /* P(n) = pos - neg   */
    seq_t numerator = (nn - 4) * diff;    /* (n-4)*P(n)         */
    const seq_t den = (seq_t)645120;
    seq_t result = numerator / den;       /* integer division    */

    return result;
}
