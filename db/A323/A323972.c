/*
 * A323972  Number of 8 X n integer matrices (m_{i,j}) such that m_{1,1}=0,
 *          m_{8,n}=2, and all rows, columns, and falling diagonals are
 *          (weakly) monotonic without jumps of 2.
 * https://oeis.org/A323972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 70, u128 n <= 1187,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A323972
   Formula: a(n) = Σ_{k=0}^{17} c_k * binom(n+16-k , 16)
   where the coefficient c_k are those of the polynomial
   P(x) = 6x^17 – 95x^16 + 707x^15 – 3278x^14 + 10588x^13
          – 25239x^12 + 45878x^11 – 64775x^10 + 71619x^9
          – 62024x^8 + 41650x^7 – 21151x^6 + 7977x^5
          – 1820x^4 + 343x^3 + 38x^2 + 4x + 1. */

/* The binary coefficient function is provided by the utils library. */
#include "../utils/binom.h"

/* Coefficients of P(x) in increasing order of x^k (k = 0 … 17). */
static const int coeff[18] = {
    +1,   +4,  +38,  +343, -1820, +7977, -21151, +41650,
   -62024, +71619, -64775, +45878, -25239, +10588, -3278, +707,
   -95,   +6
};

seq_t a(idx_t n)
{
    /* Only the first 18 terms of the sum are non‑zero for n<17. */
    idx_t limit = n < 17 ? n : 17;
    seq_t pos = 0;   /* sum of terms with positive coefficient   */
    seq_t neg = 0;   /* sum of absolute values of negative terms */

    for (idx_t k = 0; k <= limit; ++k) {
        /* Binomial coefficient C(n+16-k , 16).  The helper binom
           returns zero automatically when the upper argument is < 16. */
        seq_t bin = binom(n + 16 - k, 16);

        int c = coeff[k];
        if (c >= 0)
            pos += bin * (seq_t)c;
        else
            neg += bin * (seq_t)(-c);
    }

    /* The sequence is positive, so pos ≥ neg and subtraction is safe
       even for unsigned seq_t due to modular arithmetic. */
    return pos - neg;
}
