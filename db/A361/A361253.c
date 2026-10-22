/*
 * A361253  If n = m^2 for some m > 1 then a(n) = a(m), otherwise a(n) = n.
 * https://oeis.org/A361253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1. For n>1: if n is a perfect square (n=m^2 with
 *            m>1) then a(n)=a(m), else a(n)=n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Static helper: integer square root (floor) for 64‑bit unsigned integers.
 * Uses a binary‑based algorithm with 128‑bit multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (x >> 1) + 1;
    if (x < 2) return x;        /* 0 or 1 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* a(0)=0 */
    if (n == 1) return 1;   /* a(1)=1 */

    while (1) {
        idx_t r = isqrt_u64(n);              /* floor(sqrt(n)) */
        if ((unsigned __int128)r * r != n)   /* not a perfect square */
            break;
        if (r == 1)                          /* reached 1 */
            return 1;
        n = r;                                /* recurse on the root */
    }
    return (seq_t)n;                          /* n is not a square */
}
