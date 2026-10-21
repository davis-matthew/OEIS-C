/*
 * A250015  Number of length 1+5 0..n arrays with no six consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 3*n^5 + 5*n^4 + 5*n^3 + 4*n^2 + 2*n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses Horner's rule to avoid large intermediate values. */
seq_t a(idx_t n)
{
        /* n is assumed >= 1; for n == 0 the value is undefined (return 0). */
        if (n == 0) return 0;

        seq_t ns = n;
        seq_t r  = (seq_t)1;          /* coefficient of n^5          */
        r = r * ns + (seq_t)3;        /* +3*n^4                      */
        r = r * ns + (seq_t)5;        /* +5*n^3                      */
        r = r * ns + (seq_t)5;        /* +5*n^2                      */
        r = r * ns + (seq_t)4;        /* +4*n                        */
        r = r * ns + (seq_t)2;        /* +2 (constant term)          */

        return ns * r;                /* multiply by the leading n   */
}
