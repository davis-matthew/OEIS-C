/*
 * A257931  Period 24 sequence [0, 1, 1, 0, -2, -1, 0, 1, 0, 0, -1, -1, 0, 1,
 *          1, 0, 0, -1, 0, 1, 2, 0, -1, -1].
 * https://oeis.org/A257931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = coeff of x/(1 - x + x^2) - x^4/(1 - x^4 + x^8)
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the two-periodic rational generating function
 * described in the OEIS entry.  The coefficients of the first
 * part G(x)=x/(1-x+x²) repeat with period 6: [0,1,1,0,-1,-1].
 * The coefficients of the second part H(x)=x⁴/(1-x⁴+x⁸) are zero
 * except at indices n≡0 (mod 4), where they are the
 * coefficients of 1/(1-y+y²) evaluated at y=(x⁴)ⁱ, i.e. a pattern
 * of period 6: [1,1,0,-1,-1,0].  Subtracting the two gives the
 * sequence of period 24 shown in the OEIS.  This function
 * computes a(n) in O(1) time for any n ≥ 0 using these
 * periodic patterns; no table of results is hard‑coded. */

seq_t a(idx_t n)
{
    /* periodic pattern of coefficients of G(x)=x/(1-x+x²) */
    static const seq_t gtab[6] = {0, 1, 1, 0, -1, -1};
    /* periodic pattern of coefficients of 1/(1-y+y²), used in H(x) */
    static const seq_t ctab[6] = {1, 1, 0, -1, -1, 0};

    seq_t g = gtab[n % 6];

    seq_t h = 0;
    if (n >= 4 && (n & 3ULL) == 0) {            /* n % 4 == 0 */
        idx_t m = (n - 4) >> 2;                 /* m = (n-4)/4   */
        h = ctab[m % 6];
    }

    return g - h;
}
