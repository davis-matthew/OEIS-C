/*
 * A241747  Triangle read by rows: T(n,k) = (4*n+3)*(4*k+3).
 * https://oeis.org/A241747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (4*r+3)*(4*k+3), where r = floor((sqrt(8*n+1)-1)/2) (the
 *            row number) k = n - r*(r+1)/2 (column within that row)
 * Range:     u8 n <= 12, u16 n <= 2142, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       For the small n values required (up to about 10 000) a simple
       linear search is fast enough and avoids any reliance on
       floating‑point or external integer‑sqrt routines. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        ++r;
    --r;                                 /* r is now the desired row */

    /* Offset of the first element of row r in the flattened sequence */
    idx_t offset = (r * (r + 1)) / 2;

    /* Column index k within that row */
    idx_t k = n - offset;

    /* Compute the triangle entry (4*r+3)*(4*k+3) in the correct type */
    seq_t term_r = ((seq_t)4 * r + 3);
    seq_t term_k = ((seq_t)4 * k + 3);
    return term_r * term_k;
}
