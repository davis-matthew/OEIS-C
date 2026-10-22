/*
 * A390346  Lower (1/2)-midsequence of binomial(n,3) and (-1)*binomial(n,2);
 *          see Comments.
 * https://oeis.org/A390346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-5)/12)
 * Range:     i8 n <= 7, i16 n <= 34, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the signed integer type used for the sequence. */
    seq_t tn = (seq_t)n;

    /* Compute the numerator n*(n-1)*(n-5).  All operations are
       performed in the seq_t type, which is signed, so negative
       intermediate values are handled correctly. */
    seq_t num = tn * (tn - 1) * (tn - 5);

    /* Integer division truncates toward zero.  For negative numerators
       the true floor division requires subtracting 1 if the remainder
       is non‑zero. */
    seq_t q = num / 12;
    if (num < 0 && num % 12 != 0) {
        q -= 1;
    }
    return q;
}
