/*
 * A315384  Coordination sequence Gal.5.256.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊26*n/5⌋ + {0,1,2,0,-1} for n mod 5 = 0,1,2,3,4
 *            respectively (valid for n ≥ 4). For n < 4 the values are
 *            hand‑tabled
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Hand‑tabled values for n < 6 */
    static const seq_t init[6] = {1, 6, 11, 14, 19, 26};
    if (n < 6)
        return init[n];

    /* Main formula for n ≥ 6 */
    seq_t n_seq = (seq_t)n;
    seq_t base = ((seq_t)26 * n_seq) / 5;   /* floor(26*n/5) */

    /* Correction based on n mod 5 */
    unsigned r = n % 5;
    switch (r) {
        case 0: return base;             /* +0 */
        case 1: return base + 1;         /* +1 */
        case 2: return base + 2;         /* +2 */
        case 3: return base;             /* +0 */
        case 4: return base - 1;         /* -1 */
    }
    /* Should never reach here */
    return base;
}
