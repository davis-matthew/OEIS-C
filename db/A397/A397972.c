/*
 * A397972  Lower (1,1/3) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n+1)/2 + (n*n)/3 )
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* all computations in the type seq_t to avoid truncation issues and
       to work with arbitrary‑precision builds. */
    seq_t n_tr = (seq_t)n;                 /* cast to seq_t */
    seq_t tri = (n_tr * (n_tr + (seq_t)1)) / (seq_t)2;
    seq_t sq  = (n_tr * n_tr) / (seq_t)3;
    return tri + sq;
}
