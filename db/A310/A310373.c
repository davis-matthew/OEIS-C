/*
 * A310373  Coordination sequence Gal.6.150.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (9 * n + e(n)) / 2 * where e(n) is a small adjustment
 *            depending on n mod 12 * for n > 0 : e(n) = {0, -1, 2, 1, 0, 1,
 *            0, -1, 0, -1, -2, 1} * for n == 0 : e(0) = 2 * (this yields the
 *            sequence 1,4,10,14,18,23,27,31,36,40,44,50,54,…)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /*  n == 0 is a special case: a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* e(n) for n > 0, indexed by (n mod 12) */
    static const int offset[12] = {
        0,  /* 0 mod 12 → e = 0 for n > 0   */
        -1, /* 1 mod 12 */
        2,  /* 2 mod 12 */
        1,  /* 3 mod 12 */
        0,  /* 4 mod 12 */
        1,  /* 5 mod 12 */
        0,  /* 6 mod 12 */
        -1, /* 7 mod 12 */
        0,  /* 8 mod 12 */
        -1, /* 9 mod 12 */
        -2, /*10 mod 12 */
        1   /*11 mod 12 */
    };

    int e = offset[n % 12];

    /* Use 128‑bit arithmetic to avoid overflow in intermediate
     * calculation for 64‑bit seq_t builds. */
    __int128 tmp = (__int128)n * 9 + e;  /* 9*n + e(n) */
    return (seq_t)(tmp / 2);             /* divide now that it is even */
}
