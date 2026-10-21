/*
 * A271638  The total sum of the cubes of all parts of all compositions of n.
 * https://oeis.org/A271638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (13*n - 36)*2^(n - 1) + 6*n + 18
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t seq_n = (seq_t)n;          /* convert index to seq_t */
    seq_t pow2 = (seq_t)1;          /* 2^(n-1) */

    /* Compute 2^(n-1) by repeated multiplication */
    for (idx_t i = 1; i < n; ++i) {
        pow2 *= (seq_t)2;
    }

    /* (13*n - 36)*2^(n-1) */
    seq_t term1 = (seq_t)13 * seq_n; /* 13*n */
    term1 -= (seq_t)36;              /* 13*n - 36 */
    term1 *= pow2;                   /* times 2^(n-1) */

    /* 6*n + 18 */
    seq_t term2 = (seq_t)6 * seq_n + (seq_t)18;

    return term1 + term2;
}
