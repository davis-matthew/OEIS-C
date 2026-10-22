/*
 * A312472  Coordination sequence Gal.6.149.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 2 + 4*⌊(n-3)/11⌋ + 2*I((n-3)%11 ≥ 6) with special
 *            cases a(0)=1, a(1)=4, a(2)=8
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;
    seq_t N = (seq_t)n;
    idx_t n3  = n - 3;          /* n ≥ 3 here */
    idx_t p   = n3 / 11;        /* number of full 11‑step cycles */
    idx_t r   = n3 % 11;        /* position inside the current cycle */
    seq_t pp  = (seq_t)p;
    seq_t offset = 2 + 4 * pp;  /* base offset for the cycle (n3<6) */
    if (r >= 6) offset += 2;    /* extra +2 for the last 5 positions */
    return N * 4 + offset;
}
