/*
 * A394769  Number of transitive relations on an n-set with at most six
 *          ordered pairs.
 * https://oeis.org/A394769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (720 - 598956*n + 2134012*n^2 - 3281559*n^3 + 2912954*n^4
 *            * - 1672444*n^5 + 657487*n^6 - 182176*n^7 + 35980*n^8 * -
 *            5035*n^9 + 486*n^10 - 30*n^11 + n^12)/720
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial, in ascending order of powers. */
    static const seq_t coeff[13] = {
        720,
        -598956,
        2134012,
        -3281559,
        2912954,
        -1672444,
        657487,
        -182176,
        35980,
        -5035,
        486,
        -30,
        1
    };

    seq_t res = coeff[12];            /* start with the leading coefficient (n^12) */
    for (int i = 11; i >= 0; --i) {   /* Horner's method, highest to lowest */
        res = res * (seq_t)n + coeff[i];
    }
    return res / 720;                 /* division is exact by definition */
}
