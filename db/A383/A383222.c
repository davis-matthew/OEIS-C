/*
 * A383222  Coefficient of x^4 in expansion of (x+2) * (x+5) * ... *
 *          (x+3*n-1).
 * https://oeis.org/A383222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^4 in ∏_{k=1}^n (x + 3k – 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute with a rolling 5‑term coefficient array; uses 128‑bit integers for safety */

#include <stdint.h>

seq_t a(idx_t n)
{
    typedef unsigned __int128 big;
    big coeff[5] = {1, 0, 0, 0, 0};   /* coeff[0] = constant term of the current product */

    for (idx_t i = 1; i <= n; ++i) {
        big ai = (big)(3 * i - 1);       /* the constant term of the new factor */

        /* update from the highest degree down to 1 */
        for (int k = 4; k >= 1; --k)
            coeff[k] = ai * coeff[k] + coeff[k - 1];

        coeff[0] = ai * coeff[0];       /* constant term of the new product   */
    }

    return (seq_t)coeff[4];             /* coefficient of x^4 */
}
