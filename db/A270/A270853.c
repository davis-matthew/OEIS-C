/*
 * A270853  Number of 4X4X4 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n+1, n or n-1 exactly once.
 * https://oeis.org/A270853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 15, * if n is even: a(n) = 54*n^9 - 2304*n^8 + 48114*n^7
 *            - 640560*n^6 * + 5967942*n^5 - 40260606*n^4 + 196350792*n^3 * -
 *            666434964*n^2 + 1425184728*n - 1458389412 * if n is odd : a(n) =
 *            54*n^9 - 2304*n^8 + 48141*n^7 - 641856*n^6 * + 5994894*n^5 -
 *            40583919*n^4 + 198797448*n^3 * - 678190998...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 11, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Table of first 15 terms */
    static const seq_t table[15] = {
        0ULL, 0ULL, 252ULL, 6822ULL, 108348ULL,
        1144464ULL, 8559378ULL, 49511370ULL,
        225803628ULL, 868915644ULL, 2866094766ULL,
        8402366160ULL, 22207133946ULL, 53934812124ULL,
        121777854570ULL
    };

    if (n <= 15) {
        return table[n - 1];
    }

    /* Coefficients for even and odd polynomials (signed 64‑bit) */
    static const int64_t coeffEven[10] = {
        54LL,          -2304LL,          48114LL,
        -640560LL,     5967942LL,        -40260606LL,
        196350792LL,   -666434964LL,     1425184728LL,
        -1458389412LL
    };
    static const int64_t coeffOdd[10]  = {
        54LL,          -2304LL,          48141LL,
        -641856LL,     5994894LL,        -40583919LL,
        198797448LL,   -678190998LL,     1458498405LL,
        -1501345395LL
    };
    const int64_t *coeffs = (n & 1ULL) ? coeffOdd : coeffEven;

    /* Compute n^k for k = 0..9 and accumulate positive & negative contributions */
    seq_t power = 1;           /* n^0 */
    seq_t sumPos = 0, sumNeg = 0;

    for (int k = 0; k <= 9; ++k) {
        int64_t coeff = coeffs[9 - k];   /* coeff of n^k */
        if (coeff >= 0) {
            sumPos += (seq_t)(coeff) * power;
        } else {
            sumNeg += (seq_t)(-coeff) * power;
        }
        power *= (seq_t)n;           /* Advance to n^(k+1) */
    }

    /* Since the final result is known to be non‑negative, subtraction is safe */
    return sumPos - sumNeg;
}
