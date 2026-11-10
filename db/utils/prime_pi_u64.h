/* prime_pi_u64: pi(x), the number of primes <= x, by the Lucy_Hedgehog method
 * in O(x^(3/4)) time and O(sqrt(x)) memory (pi(10^12) in well under a second).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_PRIME_PI_U64_H
#define OEISC_UTILS_PRIME_PI_U64_H
#include <stdint.h>
#include <stdlib.h>

/* S(v) = the number of integers in 2..v not crossed off by the primes so far,
   for the O(sqrt x) values v = x/i: lo[v] holds S(v) for v <= r = isqrt(x),
   hi[i] holds S(x/i). Sieving by each prime p <= r takes
   S(v) -= S(v/p) - S(p-1) for v >= p^2; at the end S(x) = pi(x). */
static uint64_t prime_pi_u64(uint64_t x)
{
    if (x < 2) return 0;
    uint64_t r = 0;                          /* r = isqrt(x), bit by bit */
    for (uint64_t b = (uint64_t)1 << 31; b; b >>= 1)
        if (r + b <= x / (r + b)) r += b;
    uint64_t *lo = (uint64_t *)malloc((r + 2) * sizeof *lo);
    uint64_t *hi = (uint64_t *)malloc((r + 2) * sizeof *hi);
    if (!lo || !hi) abort();
    for (uint64_t i = 1; i <= r; ++i) {
        lo[i] = i - 1;
        hi[i] = x / i - 1;
    }
    for (uint64_t p = 2; p <= r; ++p) {
        if (lo[p] == lo[p - 1]) continue;    /* p is not prime */
        uint64_t pc = lo[p - 1], p2 = p * p;
        uint64_t end = x / p2 < r ? x / p2 : r;
        for (uint64_t i = 1; i <= end; ++i) {
            uint64_t d = i * p;
            hi[i] -= (d <= r ? hi[d] : lo[x / d]) - pc;
        }
        for (uint64_t v = r; v >= p2; --v) lo[v] -= lo[v / p] - pc;
    }
    uint64_t res = hi[1];
    free(lo);
    free(hi);
    return res;
}

#endif
