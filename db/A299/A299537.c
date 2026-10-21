/*
 * A299537  Number of ways to write n^2 as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers and z <= w such that x or y is a power of 4
 *          (including 4^0 = 1) and x + 3*y is also a power of 4.
 * https://oeis.org/A299537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_power_of_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A299537: number of ways to write n^2 as
 *           x^2 + y^2 + z^2 + w^2 with
 *           x,y,z,w ≥ 0, z ≤ w,
 *           at least one of x,y is a power of 4 (4^0 = 1),
 *           and x + 3*y is also a power of 4.      */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_power_of_4.h"

/* --------- integer square root ------------------------------------ */
static inline uint32_t isqrt_uint64(uint64_t v)
{
    uint32_t lo = 0, hi = 1;
    while ((uint64_t)hi * hi <= v) {
        if (hi > 0x80000000u) { /* avoid overflow of hi*hi */
            hi = 0xFFFFFFFFu;
            break;
        }
        hi <<= 1;
    }
    while (lo < hi) {
        uint32_t mid = (lo + hi + 1) >> 1;
        uint64_t sq = (uint64_t)mid * mid;
        if (sq <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* --------- prime sieve (once) ------------------------------------ */
static bool primes_init = false;
static uint64_t primes[100000];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_init) return;
    const size_t LIMIT = 100000;
    static bool sieve[100001] = { false };
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    sieve[j] = true;
            }
        }
    }
    primes_init = true;
}

/* --------- number of unordered non‑negative solutions z,w ------ */
static inline seq_t count_rep(uint64_t M)
{
    if (M == 0ULL) return (seq_t)1;   /* (0,0) */

    /* flags for special cases */
    uint64_t sqrtM = isqrt_uint64(M);
    bool is_sq = (sqrtM * sqrtM == M);
    bool is_twix_sq = false;
    if ((M & 1ULL) == 0ULL) {
        uint64_t half = M >> 1;
        uint64_t sh = isqrt_uint64(half);
        is_twix_sq = (sh * sh == half);
    }

    /* factorisation for r2(n) */
    init_primes();
    uint64_t temp = M;
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0ULL) {
            unsigned exp = 0;
            while (temp % p == 0ULL) {
                temp /= p;
                exp++;
            }
            if ((p % 4ULL) == 3ULL) {
                if (exp & 1U) return (seq_t)0;
            } else if ((p % 4ULL) == 1ULL) {
                prod *= (seq_t)(exp + 1);
            }
        }
    }
    if (temp > 1ULL) {
        if ((temp % 4ULL) == 3ULL) return (seq_t)0;
        if ((temp % 4ULL) == 1ULL) prod *= (seq_t)2;
        /* prime 2 contributes nothing special */
    }

    seq_t r2 = (seq_t)4 * prod;
    seq_t t = (r2 +
               (seq_t)(is_twix_sq ? 4 : 0) +
               (seq_t)(is_sq      ? 4 : 0))
              / (seq_t)8;
    return t;
}

/* --------- main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* powers of 4 up to 4*n */
    idx_t powers[64];
    size_t pcnt = 0;
    idx_t val = 1;
    while (val <= 4 * n && pcnt < sizeof(powers)/sizeof(powers[0])) {
        powers[pcnt++] = val;
        if (val > (UINT64_MAX / 4)) break;
        val *= 4ULL;
    }

    seq_t total = (seq_t)0;
    uint64_t n2 = (uint64_t)n * (uint64_t)n;

    /* case 1: x is a power of 4 */
    for (size_t ix = 0; ix < pcnt; ++ix) {
        idx_t x = powers[ix];
        if (x > n) continue;
        for (size_t iq = 0; iq < pcnt; ++iq) {
            idx_t q = powers[iq];
            if (q < x) continue;
            uint64_t diff = (uint64_t)q - x;
            if (diff % 3U) continue;
            idx_t y = diff / 3U;
            if (y > n) continue;

            uint64_t xsq = (uint64_t)x * (uint64_t)x;
            uint64_t ysq = (uint64_t)y * (uint64_t)y;
            if (xsq + ysq > n2) continue;

            uint64_t M = n2 - xsq - ysq;
            total += count_rep(M);
        }
    }

    /* case 2: y is a power of 4 but x is not (to avoid double counting) */
    for (size_t iy = 0; iy < pcnt; ++iy) {
        idx_t y = powers[iy];
        if (y > n) continue;
        for (size_t iq = 0; iq < pcnt; ++iq) {
            idx_t q = powers[iq];
            if (q < 3 * y) continue;
            idx_t x = q - 3U * y;
            if (x > n) continue;
            if (is_power_of_4(x, powers, pcnt)) continue; /* already counted */

            uint64_t xsq = (uint64_t)x * (uint64_t)x;
            uint64_t ysq = (uint64_t)y * (uint64_t)y;
            if (xsq + ysq > n2) continue;

            uint64_t M = n2 - xsq - ysq;
            total += count_rep(M);
        }
    }

    return total;
}
