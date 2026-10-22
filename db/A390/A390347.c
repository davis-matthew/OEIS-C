/*
 * A390347  Upper (1/2)-midsequence of binomial(n,3) and (-1)*binomial(n,2);
 *          see Comments.
 * https://oeis.org/A390347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 34, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = ceil( n*(n-1)*(n-5) / 12 ) */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * (nn - 1) * (nn - 5);
    if (num >= 0)
        return (num + 11) / 12;
    else
        return num / 12;   /* for negative numerators, truncation is ceil */
}
