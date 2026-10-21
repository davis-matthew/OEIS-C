/*
 * A283305  List points (x,y) having integer coordinates with x >= 0, y >= 0,
 *          sorted first by x^2+y^2 and in case of a tie, by x-coordinate.
 *          Sequence gives x-coordinates.
 * https://oeis.org/A283305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the x‑coordinate of the n‑th lattice point (x,y) with x,y
 *            ≥ 0 sorted by x^2 + y^2 and then by x
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square‑root using 128‑bit arithmetic to avoid overflow */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    for (int k = 63; k >= 0; --k) {
        idx_t t = r | ((idx_t)1 << k);
        if ((unsigned __int128)t * t <= n)
            r = t;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t r = 0; ; ++r) {
        for (idx_t x = 0; (unsigned __int128)x * x <= r; ++x) {
            idx_t y2 = r - (unsigned __int128)x * x;
            idx_t y = isqrt_u64(y2);
            if ((unsigned __int128)y * y == y2) {
                ++count;
                if (count == n)
                    return (seq_t)x;
            }
        }
    }
}
