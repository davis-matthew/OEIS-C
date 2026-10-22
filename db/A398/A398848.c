/*
 * A398848  Lower (-1/3, 1/2)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(7*n-5)/12 )
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the numerator would underflow a
       unsigned multiplication, but the correct result is
       0, so handle these cases explicitly. */
    if (n < 2)
        return 0;

    /* Convert n to seq_t once to avoid repeated casts. */
    seq_t nn = (seq_t)n;

    /* Compute numerator: n * (7*n - 5).  For n ≥ 2
       the inner term 7*n-5 is non‑negative, so all
       operations stay within the unsigned range of
       seq_t (or the arbitrary‑precision implementation). */
    seq_t num = nn * ((seq_t)7 * nn - (seq_t)5);

    /* Perform the floor division by 12.  All values are
       non‑negative, so integer division already yields
       floor. */
    return num / (seq_t)12;
}
