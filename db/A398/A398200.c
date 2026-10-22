/*
 * A398200  Lower (4/5,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n*n + 2*n)/5)
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (lower (4/5,1) midsequence of triangles and squares) */

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic to avoid overflow on small seq_t types */
    unsigned long long nn   = (unsigned long long)n;
    unsigned long long num  = 7ULL * nn * nn + 2ULL * nn;
    return (seq_t)(num / 5ULL);
}
