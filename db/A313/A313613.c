/*
 * A313613  Coordination sequence Gal.4.72.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 10, a(3) = 14. For n ≥ 4: let m = n –
 *            3, c = ⌊m / 7⌋, r = m mod 7. a(n) = 14 + 34·c + p[r] where
 *            p[0…6] = {0,6,10,15,20,25,30} are the partial sums of the
 *            7‑cycle of differences 6,4,5,5,5,5,4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the coordination sequence A313613, indexed from n=0. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)14;

    /* Partial sums of the 7‑cycle differences */
    static const unsigned char partial[8] = {
        0,   /* r = 0 */
        6,   /* 6           -> 6 */
        10,  /* 6+4         ->10 */
        15,  /* +5          ->15 */
        20,  /* +5          ->20 */
        25,  /* +5          ->25 */
        30,  /* +5          ->30 */
        34   /* +4          ->34 */
    };

    /* m = n-3 ≥ 1 for n≥4 */
    idx_t m    = n - 3;
    idx_t cycle = m / 7;      /* number of full cycles */
    idx_t rem   = m % 7;      /* position inside the current cycle */

    seq_t result = (seq_t)14;
    result += (seq_t)34 * (seq_t)cycle;   /* add full cycles */
    result += (seq_t)partial[rem];        /* add remainder part */

    return result;
}
