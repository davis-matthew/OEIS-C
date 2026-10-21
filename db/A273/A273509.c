/*
 * A273509  Orders of the symmetry groups of the six convex regular
 *          4-polytopes, in the order 5-cell, 8-cell, 16-cell, 24-cell,
 *          120-cell, 600-cell.
 * https://oeis.org/A273509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = order of the symmetry group of the nth convex regular
 *            4‑polytope (5‑cell, 8‑cell, 16‑cell, 24‑cell, 120‑cell,
 *            600‑cell). For n>6, a(n)=0
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        /* 5‑cell: 5! */
        seq_t val = 1;
        for (idx_t i = 2; i <= 5; ++i)
            val *= (seq_t)i;
        return val;
    } else if (n == 2 || n == 3) {
        /* 8‑cell and 16‑cell: 2^4 * 4! = 16 * 24 = 384 */
        return (seq_t)16 * (seq_t)24;
    } else if (n == 4) {
        /* 24‑cell: 2^7 * 3^2 = 128 * 9 = 1152 */
        return (seq_t)128 * (seq_t)9;
    } else if (n == 5 || n == 6) {
        /* 120‑cell and 600‑cell: 2^6 * 3^2 * 5^2 = 64 * 9 * 25 = 14400 */
        return (seq_t)64 * (seq_t)9 * (seq_t)25;
    } else {
        return (seq_t)0;
    }
}
