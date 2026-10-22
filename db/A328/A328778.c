/*
 * A328778  Number of indecomposable closed walks of length 2n along the edges
 *          of a cube based at a vertex.
 * https://oeis.org/A328778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(0)=1, a(1)=3, and for n >= 2, a(n) = 12 * 7^(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* compute 7^(n-2) and multiply by 12 */
    seq_t pow7 = (seq_t)1;
    seq_t base  = (seq_t)7;
    idx_t exp = n - 2;

    for (idx_t i = 0; i < exp; ++i)
        pow7 *= base;

    return (seq_t)12 * pow7;
}
