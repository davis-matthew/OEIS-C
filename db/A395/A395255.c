/*
 * A395255  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x, y, z are primes satisfying x <= y <= z.
 * https://oeis.org/A395255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime triples (x,y,z) with x^2 + y*z = n and x
 *            <= y <= z. This function counts those triples for any
 *            non‑negative integer n
 * Range:     u8 n <= 9773, u16 n <= 9773, u32 n <= 9773, u64 n <= 9773,
 *            u128 n <= 9773, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

static idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;

    if (n <= 1) /* no positive solution exists */
        return (seq_t)count;

    idx_t sqrt_n = isqrt(n);

    for (idx_t x = 2; x <= sqrt_n; ++x) {
        if (!is_prime(x))
            continue;

        idx_t prod = x * x;
        if (prod > n)
            continue;

        idx_t m = n - prod;
        if (m < 2)
            continue;

        idx_t sqrt_m = isqrt(m);

        for (idx_t y = x; y <= sqrt_m; ++y) {
            if (!is_prime(y))
                continue;
            if (m % y)
                continue;

            idx_t z = m / y;
            if (z < y)
                continue;               /* guarantees y <= z */

            if (!is_prime(z))
                continue;

            ++count;
        }
    }

    return (seq_t)count;
}
