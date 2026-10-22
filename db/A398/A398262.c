/*
 * A398262  Number of indecomposable closed walks of length 2n along the edges
 *          of the 4-cube (hypercube Q_4, the tesseract) based at a vertex.
 * https://oeis.org/A398262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=24; a(n)=16*a(n-1)-24*a(n-2) for n>2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the defined initial values */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)4;
    if (n == 2u) return (seq_t)24;

    /* For n>=3 use the linear recurrence iteratively */
    seq_t prev2 = (seq_t)4;   /* a(1) */
    seq_t prev1 = (seq_t)24;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur   = (seq_t)16 * prev1 - (seq_t)24 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;  /* a(n) */
}
