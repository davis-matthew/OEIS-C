/*
 * A310411  Coordination sequence Gal.4.72.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4. For n≥2, let the increment pattern D =
 *            [6,4,6,4,6,4,4]. Then a(n) = 4 + Σ_{k=0}^{n-2} D[k mod 7].
 *            Equivalently, let c = (n-1)/7, r = (n-1)%7, and P =
 *            [0,6,10,16,20,26,30] be the prefix sums of D: a(n) = 4 + c*34 +
 *            P[r]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    idx_t count = n - 1;          /* number of increments to add */
    idx_t c = count / 7;          /* complete 7‑step cycles of the pattern */
    idx_t r = count % 7;          /* remainder of the pattern */

    /* Prefix sums of the pattern D */
    static const seq_t P[7] = {0, 6, 10, 16, 20, 26, 30};

    seq_t sum = (seq_t)c * (seq_t)34 + P[r];
    return (seq_t)4 + sum;
}
