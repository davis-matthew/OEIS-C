/*
 * A397433  a(n) is the number of integers k >= 0 such that n has a divisor in
 *          the interval [2^k, 2^(k+1)).
 * https://oeis.org/A397433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of distinct bit‑lengths of the divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: bit‑length of d is floor(log2(d)) + 1, so we check d ≥ 1 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Portable popcount for 64‑bit values */
static inline unsigned int popcount64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

/* Portable floor(log2) for 64‑bit values */
static inline unsigned int floor_log2_uint64(uint64_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(v);
#else
    unsigned int r = 0;
    while (v >>= 1) ++r;
    return r;
#endif
}

/* Recursive enumeration of all divisors of n, marking the bit‑index
   corresponding to each divisor's bit‑length in the mask. */
static void gen_divs_rec(const uint64_t *pr,
                         const uint64_t *ex,
                         size_t idx, size_t total,
                         uint64_t cur, uint64_t *mask)
{
    if (idx == total) {
        unsigned int k = floor_log2_uint64(cur);
        *mask |= ((uint64_t)1) << k;
        return;
    }
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divs_rec(pr, ex, idx + 1, total, cur * mult, mask);
        mult *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term */
    uint64_t nn = (uint64_t)n;
    /* Factorisation storage (unlikely to need more than 64 distinct primes) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t primeCnt = 0;
    factor_u64(nn, primes, exps, &primeCnt);

    uint64_t mask = 0;
    gen_divs_rec(primes, exps, 0, primeCnt, 1ULL, &mask);

    return (seq_t)popcount64(mask);
}
