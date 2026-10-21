/*
 * A247390  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (12345678910)*.
 * https://oeis.org/A247390
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coeff[(n-2) mod 10], where coeff[0..9] are the
 *            coefficients of the numerator polynomial in the generating
 *            function (x^2 * (41 + 40*x + 21*x^2 + 11*x^3 + 11*x^4 + 40*x^5 +
 *            41*x^6 + 20*x^7 + 11*x^8 + 10*x^9)) / (1 - x^10). The numerator
 *            coefficients are 41, 40, 21, 11, 11, 40, 41, 20, 11, 10...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for undefined indices < 2 (behaviour unspecified). */
    if (n < 2)
        return 0;

    /* Coefficients of the numerator polynomial (see comment above). */
    static const seq_t coeff[10] = {
        41, /* coefficient of x^0  in Q(x), i.e. for n = 2          */
        40, /* for n = 3          */
        21, /* for n = 4          */
        11, /* for n = 5          */
        11, /* for n = 6          */
        40, /* for n = 7          */
        41, /* for n = 8          */
        20, /* for n = 9          */
        11, /* for n =10          */
        10  /* for n =11          */
    };

    /* Index within the period: (n - 2) mod 10.  All types are unsigned,
       so subtraction and modulo are safe for n >= 2. */
    idx_t i = (n - 2) % 10;
    return coeff[i];
}
