/*
 * A302930  Maximum number of 6's possible in an infinite Minesweeper grid
 *          with n mines.
 * https://oeis.org/A302930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 for n < 6; a(n) = n + 1 – floor(sqrt(8*n – 1)) for n ≥
 *            6
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 64‑bit unsigned integers */
static inline idx_t int_sqrt_u64(idx_t v) {
    idx_t r = 0;
    idx_t bit = 1ULL << 62;              /* highest power of 4 ≤ 2^64-1 */
    while (bit > v) bit >>= 2;
    while (bit != 0) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 6ULL)
        return 0;
    idx_t v = 8ULL * n - 1ULL;
    idx_t s = int_sqrt_u64(v);          /* floor(sqrt(8*n-1)) */
    return (seq_t)(n + 1ULL - s);
}
