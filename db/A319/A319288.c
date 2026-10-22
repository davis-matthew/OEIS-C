/*
 * A319288  a(n) is the smallest k such that A319284(n, k) >= A319284(n, j)
 *          for all 0 <= j <= n.
 * https://oeis.org/A319288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n - floor(sqrt(n)) - I(n = m^2 - 1) where I is the
 *            indicator function (1 if true, 0 otherwise)
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 64‑bit unsigned values (floor(sqrt(x))). */
static idx_t isqrt_u64(idx_t x)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;     /* highest power of four <= 2^64 */

    while (bit > x)
        bit >>= 2;

    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t s = isqrt_u64(n);
    /* check whether n == (s+1)^2 - 1, i.e. n+1 is a perfect square */
    unsigned __int128 sq = (unsigned __int128)(s + 1) * (s + 1);
    idx_t diff = (sq == (unsigned __int128)n + 1) ? s + 1 : s;
    return (seq_t)n - (seq_t)diff;
}
