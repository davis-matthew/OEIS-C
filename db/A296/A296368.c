/*
 * A296368  Coordination sequence for the Cairo or dual-3.3.4.3.4 tiling with
 *          respect to a trivalent point.
 * https://oeis.org/A296368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 8, for n ≥ 3: a(n) = 4*n if n is odd,
 *            a(n) = 4*n – 1 if n ≡ 0 (mod 4), a(n) = 4*n + 1 if n ≡ 2 (mod 4)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;
    seq_t m = (seq_t)n;          /* convert once for power‑of‑2 operations */
    if (n & 1ULL)                 /* odd */
        return (seq_t)4 * m;
    else if ((n & 3ULL) == 0)     /* n ≡ 0 (mod 4) */
        return (seq_t)4 * m - (seq_t)1;
    else                          /* n ≡ 2 (mod 4) */
        return (seq_t)4 * m + (seq_t)1;
}
