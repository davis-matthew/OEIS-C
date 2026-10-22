/*
 * A376756  Number of pairs 0 <= x <= y <= n-1 such that x^2 + x*y + y^2 == 0
 *          (mod n).
 * https://oeis.org/A376756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force counting of unordered pairs (x,y) with 0 ≤ x ≤ y < n
   that satisfy x² + xy + y² ≡ 0 (mod n). */
seq_t a(idx_t n)
{
    /* Return 1 for n == 1 (the only pair is (0,0)). */
    if (n == 0) return (seq_t)0;  /* Offset 1; this case is unused. */

    idx_t count = 0;
    for (idx_t y = 0; y < n; ++y) {
        idx_t y2 = y * y;
        for (idx_t x = 0; x <= y; ++x) {
            /* Compute (x² + xy + y²) modulo n. */
            idx_t val = (x * x + x * y + y2) % n;
            if (val == 0) ++count;
        }
    }
    return (seq_t)count;
}
