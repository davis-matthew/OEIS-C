/*
 * A313899  Coordination sequence Gal.6.620.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, * a(1) = 5, a(2) = 10, a(3) = 16, a(4) = 21, a(5) =
 *            27, a(6) = 33, * a(7) = 39, a(8) = 44, a(9) = 50, a(10) = 55,
 *            a(11) = 60; * for all n ≥ 12, a(n) = a(n‑11) + 60
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) */

    /* First 12 terms, as a small lookup table */
    static const seq_t base[12] = {
        1,   /* a(0) */
        5,   /* a(1) */
        10,  /* a(2) */
        16,  /* a(3) */
        21,  /* a(4) */
        27,  /* a(5) */
        33,  /* a(6) */
        39,  /* a(7) */
        44,  /* a(8) */
        50,  /* a(9) */
        55,  /* a(10) */
        60   /* a(11) */
    };

    /* Decompose n as n = 11 k + r, 0 ≤ r < 11 */
    idx_t k = n / 11;             /* number of full 11‑step blocks */
    idx_t r = n % 11;             /* remainder in the current block */

    if (r == 0) {                 /* n is a multiple of 11, but n ≥ 11 */
        return base[11] + (seq_t)(k - 1) * (seq_t)60;
    } else {
        return base[r] + (seq_t)k * (seq_t)60;
    }
}
