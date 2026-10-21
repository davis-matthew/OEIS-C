/*
 * A275161  Number of sides of a polygon formed by tiling n squares in a
 *          spiral.
 * https://oeis.org/A275161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 if n is a perfect square or n = m*(m+1) for some
 *            integer m; otherwise a(n) = 6. This matches the OEIS sequence
 *            A275161
 * Range:     u8 n <= 9518, u16 n <= 9518, u32 n <= 9518, u64 n <= 9518,
 *            u128 n <= 9518, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Integer square root (floor) for idx_t, using long double as an initial guess. */
static inline idx_t isqrt_idx(idx_t x)
{
    if (x <= 1) return x;            /* 0 or 1 return immediately */
    long double approx = sqrtl((long double)x);
    idx_t m = (idx_t)approx;         /* truncate to integer */
    /* Adjust for possible round‑off errors. */
    while ((unsigned __int128)(m + 1) * (m + 1) <= x) ++m;
    while ((unsigned __int128)m * m > x) --m;
    return m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; n==0 is unused */
    idx_t m = isqrt_idx(n);
    /* Check for perfect square. */
    if ((unsigned __int128)m * m == n) return (seq_t)4;
    /* Check for product of two consecutive integers. */
    if ((unsigned __int128)m * (m + 1) == n) return (seq_t)4;
    /* Otherwise return 6. */
    return (seq_t)6;
}
