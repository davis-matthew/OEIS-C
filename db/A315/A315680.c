/*
 * A315680  Coordination sequence Gal.5.89.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-9) + 48 for n >= 10 with a(0..8) =
 *            {1,6,12,17,22,26,31,36,42}
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence Gal.5.89.4 */

seq_t a(idx_t n) {
    /* first 9 terms */
    static const seq_t a0[9] = {1, 6, 12, 17, 22, 26, 31, 36, 42};
    /* terms 9..17 computed once */
    static const seq_t a1[9] = {48, 54, 60, 65, 70, 74, 79, 84, 90};

    if (n < 9) return a0[n];

    idx_t m = n - 9;               /* shift to 0.. */
    idx_t k = m / 9;               /* number of full 9‑step cycles */
    idx_t r = m % 9;               /* position inside a1 */

    return a1[r] + (seq_t)48 * k;
}
