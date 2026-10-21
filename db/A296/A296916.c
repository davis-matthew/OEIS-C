/*
 * A296916  List of coefficients of reduced chromatic polynomial of
 *          icosahedron, highest order terms first.
 * https://oeis.org/A296916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^{9-n} in the reduced chromatic
 *            polynomial of the icosahedron
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t coeff[9];

    if (!init) {
        /* Chromatic polynomial of the icosahedron (degree 12). */
        const seq_t chrom[13] = {
            0,          /* x^0  */
            -121020,    /* x^1  */
            463310,     /* x^2  */
            -782108,    /* x^3  */
            780286,     /* x^4  */
            -517360,    /* x^5  */
            241605,     /* x^6  */
            -81622,     /* x^7  */
            20023,      /* x^8  */
            -3500,      /* x^9  */
            415,        /* x^10 */
            -30,        /* x^11 */
            1           /* x^12 */
        };

        seq_t d[13];
        for (int i = 0; i < 13; ++i) d[i] = chrom[i];

        /* Divisor: x^4 - 6x^3 + 11x^2 - 6x + 0 */
        const seq_t divisor[5] = {0, -6, 11, -6, 1};

        int cur = 12;
        while (cur >= 4) {
            seq_t q = d[cur];               /* leading term of current dividend */
            coeff[cur - 4] = q;              /* coefficient of x^{cur-4} in the quotient */

            /* subtract q * divisor(x) * x^{cur-4} from the dividend */
            for (int i = 0; i < 5; ++i) {
                d[cur - 4 + i] -= q * divisor[i];
            }
            --cur;
        }
        /* coeff[8] = coefficient of x^8 (a(1)), coeff[0] = constant term (a(9)) */
        init = true;
    }

    if (n == 0) return 0;          /* undefined for n < 1 */
    if (n <= 9) return coeff[9 - n];
    return 0;                      /* beyond defined terms */
}
