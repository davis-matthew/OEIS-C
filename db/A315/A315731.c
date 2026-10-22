/*
 * A315731  Coordination sequence Gal.5.301.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - offset(n) offset(n) = 4*floor((n-1)/9) + delta,
 *            where delta = 0 if (n-1)%9 ≤ 2, delta = 2 if (n-1)%9 ≤ 4, delta
 *            = 4 otherwise
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /** Return the n-th term of OEIS sequence A315731.  */
    if (n == 0) {
        return 1;                        /* a(0) = 1  */
    }
    idx_t k = (n - 1) / 9;              /* number of complete 9‑blocks  */
    idx_t r = (n - 1) % 9;              /* offset inside the current block */

    seq_t offset = (seq_t)4 * k;        /* base offset for this block      */
    if (r <= 2) {
        /* delta = 0   – nothing to add          */
    } else if (r <= 4) {
        offset += 2;                    /* delta = 2                      */
    } else {
        offset += 4;                    /* delta = 4                      */
    }

    return (seq_t)6 * n - offset;       /*  a(n) = 6*n - offset           */
}
