/*
 * A315658  Coordination sequence Gal.6.665.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6n, with special adjustments: - a(0) = 1 - if n ≡ 0 (mod
 *            6), a(n) = 6n – 2·(n/6) - if n ≡ 3 (mod 6), a(n) = 6n – 2 (for n
 *            = 3) a(n) = 6n – 4·((n–3)/6) (for n > 3) - if n ≡ 4 (mod 6),
 *            a(n) = 6n – 2 - otherwise (n ≡ 1, 2, 5 (mod 6)), a(n) = 6n
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t s = (seq_t)n;
    idx_t mod = n % 6;
    if (mod == 1 || mod == 2 || mod == 5) {
        return (seq_t)6 * s;
    }
    if (mod == 4) {
        return (seq_t)6 * s - (seq_t)2;
    }
    if (mod == 0) {
        seq_t k = (seq_t)(s / 6);
        return (seq_t)6 * s - (seq_t)2 * k;
    }
    /* mod == 3 */
    idx_t k = (n - 3) / 6;
    seq_t d = (k == 0) ? (seq_t)2 : (seq_t)4 * (seq_t)k;
    return (seq_t)6 * s - d;
}
