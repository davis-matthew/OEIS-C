/*
 * A309118  Number of tiles added at iteration n when successively, layer by
 *          layer, building a symmetric patch of a rhombille tiling around a
 *          central star of six rhombs.
 * https://oeis.org/A309118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=6, a(2)=6, a(3)=12, a(4)=18, a(5)=24, a(6)=24; for n>6:
 *            a(n) = 2*a(n-2) - a(n-4) The function works for all n >= 1 and
 *            any order of calls
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset, not used */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)18;
    if (n == 5) return (seq_t)24;
    if (n == 6) return (seq_t)24;

    /* last[0] = a(n-4), last[1] = a(n-3), last[2] = a(n-2), last[3] = a(n-1) */
    seq_t last[4];
    last[0] = (seq_t)12; /* a(3) */
    last[1] = (seq_t)18; /* a(4) */
    last[2] = (seq_t)24; /* a(5) */
    last[3] = (seq_t)24; /* a(6) */

    idx_t i;
    for (i = 7; i <= n; ++i) {
        seq_t val = (seq_t)2 * last[2] - last[0]; /* a(i) = 2*a(i-2) - a(i-4) */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = val;
    }
    return last[3]; /* a(n) */
}
