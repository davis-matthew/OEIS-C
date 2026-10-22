/*
 * A399212  Number of solutions to x^2 + 9*y^2 = n in positive integers x and
 *          y.
 * https://oeis.org/A399212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Count of positive integer solutions (x,y) to x^2 + 9*y^2 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches over possible y and checks if n - 9*y^2 is a perfect square. */

static inline idx_t isqrt(idx_t v)
{
    /* integer square root: floor(sqrt(v)) */
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;          /* starts with the highest power of 4 <= 2^64 */
    while (bit > v) bit >>= 2;
    for (; bit; bit >>= 2) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    /* For n < 10 there is no solution: x^2 + 9*y^2 ≥ 10. */
    if (n < 10) return 0;

    seq_t count = 0;
    idx_t max_y = isqrt(n / 9);

    for (idx_t y = 1; y <= max_y; ++y) {
        idx_t y2   = y * y;           /* y^2 fits in idx_t for all n ≤ 2^64-1 */
        idx_t prod = 9 * y2;          /* 9*y^2 ≤ n, fits in idx_t */
        idx_t rem  = n - prod;       /* n - 9*y^2 ≥ 0 */

        idx_t s = isqrt(rem);
        if (s > 0 && s * s == rem) { /* x must be positive */
            ++count;
        }
    }
    return count;
}
