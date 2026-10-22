/*
 * A381737  Orders k of Hermite polynomials whose maximal coefficient in
 *          absolute value appears twice.
 * https://oeis.org/A381737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(n + 2) + (n + 1)*(-1)^(n + 1) – implemented * by
 *            evaluating the expression piecewise to keep the result unsigned.
 *            * For n odd the sign of the last term is +1; for n even it is
 *            –1. * This yields the value of the OEIS sequence A381737 for any
 *            n ≥ 1
 * Range:     u8 n <= 10, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure we work with the unsigned seq_t type */
    seq_t nn = n;               /* copy n into seq_t */
    seq_t term = 2 * nn * (nn + 2);  /* base 2*n*(n+2) */

    if ((n & 1) == 0) { /* n even → subtract (n+1) */
        term -= (nn + 1);
    } else {            /* n odd  → add (n+1)   */
        term += (nn + 1);
    }
    return term;
}
