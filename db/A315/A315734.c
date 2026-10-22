/*
 * A315734  Coordination sequence Gal.4.136.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - 2*floor((n-4)/7 + 1) for n ≥ 1, a(0) = 1
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The term 6*n comes from the average step size 6, while every 7th step
   (indices 3,10,17,…) the step is 4, reducing the total by 2 each time. */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t nn = (seq_t)n;          /* convert index to seq_t for arithmetic */
    seq_t count = 0;              /* number of indices k ∈ [1,n-1] with k % 7 == 3 */

    if (n >= 4) {
        /* first such index is 3; thereafter every +7.  Count = floor((n-4)/7)+1 */
        count = (seq_t)((n - 4) / 7 + 1);
    }

    /* a(n) = 6*n - 2*count */
    return 6 * nn - 2 * count;
}
