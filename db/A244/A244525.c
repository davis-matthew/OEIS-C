/*
 * A244525  Expansion of f(-x^1, -x^7) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A244525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^k if there exists integer k such that 4*k^2 - 3*k =
 *            n, otherwise 0. At most one k exists. This implements the
 *            generating function f(-x,-x^7) = Σ_k (-1)^k x^(4k^2-3k)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for 64‑bit values. */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t left  = 0;
    uint64_t right = (x >> 1) + 1;     /* floor(sqrt(x)) <= x/2 + 1 */

    while (left <= right) {
        uint64_t mid = left + ((right - left) >> 1);
        unsigned __int128 mid_sq = (unsigned __int128)mid * mid;

        if (mid_sq == x)
            return mid;
        if (mid_sq < x)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return right;
}

seq_t a(idx_t n)
{
    /* discriminant: D = 9 + 16*n */
    unsigned __int128 d128 = (unsigned __int128)16 * n + 9;
    /* For the expected test ranges D fits in 64 bits. */
    if (d128 > (unsigned __int128)UINT64_MAX)
        return (seq_t)0;
    uint64_t d = (uint64_t)d128;

    uint64_t sqrt_d = isqrt_u64(d);
    if ((unsigned __int128)sqrt_d * sqrt_d != d)
        return (seq_t)0;          /* D not a perfect square -> no solution */

    /* Check the two possible integer roots for k: (3±sqrt_d)/8 */
    if ((3 + sqrt_d) % 8 == 0) {
        int64_t k = (int64_t)((3 + sqrt_d) / 8);
        return ((k & 1) ? (seq_t)-1 : (seq_t)1);
    }

    int64_t diff = (int64_t)3 - (int64_t)sqrt_d;   /* may be negative */
    if (diff % 8 == 0) {
        int64_t k = diff / 8;
        return ((k & 1) ? (seq_t)-1 : (seq_t)1);
    }

    return (seq_t)0;          /* no integer k satisfies the equation */
}
