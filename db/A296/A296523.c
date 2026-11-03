/*
 * A296523  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x > 0, y
 *          >= 0 and 0 <= z <= w such that 64*x^2 + 65*y^2 = q^2 for some
 *          practical number q.
 * https://oeis.org/A296523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 7843 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS definition of a(n):
   a(n) = number of quadruples (x,y,z,w) ∈ ℤ^4 with
   x > 0, y ≥ 0, 0 ≤ z ≤ w and
   n = x² + y² + z² + w²
   such that 64·x² + 65·y² = q² for some practical number q.
   The routine searches all possibilities and counts the valid ones. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Small routine to test for practicality of a 64‑bit integer */
static bool _is_practical_uint64(uint64_t n)
{
    if (n == 1) return true;
    if (n == 2) return true;  /* 2 is practical */

    /* Build a tiny prime list sufficient for 64‑bit integers */
    static uint32_t primes[300];
    static size_t prime_cnt = 0;
    if (prime_cnt == 0) {
        const size_t LIMIT = 2000;
        bool *sieve = (bool *)calloc(LIMIT + 1, 1);
        for (size_t i = 2; i <= LIMIT; ++i) {
            if (!sieve[i]) {
                primes[prime_cnt++] = (uint32_t)i;
                if (i * i <= LIMIT) {
                    for (size_t j = i * i; j <= LIMIT; j += i)
                        sieve[j] = true;
                }
            }
        }
        free(sieve);
    }

    uint64_t sum_sigma = 1;          /* product of σ(p^e) terms */
    uint64_t temp = n;
    bool first = true;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p != 0) continue;
        unsigned int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        if (first) {
            if (p != 2) return false;   /* first prime must be 2 */
            first = false;
        } else {
            if ((uint64_t)p > sum_sigma + 1) return false;
        }

        /* compute σ(p^exp) = (p^(exp+1)-1)/(p-1) */
        uint64_t pow = 1;
        for (unsigned int k = 0; k < exp + 1; ++k) pow *= (uint64_t)p;
        uint64_t sigma = (pow - 1) / (p - 1);
        sum_sigma *= sigma;
    }

    if (temp > 1) {                          /* leftover prime factor */
        uint32_t p = (uint32_t)temp;
        if (first) return false;
        if ((uint64_t)p > sum_sigma + 1) return false;
        sum_sigma *= (uint64_t)(p + 1);       /* σ(p) = p+1 */
    }

    return true;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* Main sequence function ------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)                       /* off‑offset; harmless output */
        return 0;

    seq_t total = 0;

    /* Iterate over admissible x > 0 */
    for (idx_t x = 1; (seq_t)x * x <= n; ++x) {
        seq_t x2 = (seq_t)x * x;
        seq_t base64 = (seq_t)64 * x2;               /* 64·x² */

        /* Iterate over admissible y ≥ 0 */
        for (idx_t y = 0; (seq_t)y * y <= n - x2; ++y) {
            seq_t y2 = (seq_t)y * y;
            seq_t remaining = n - x2 - y2;

            /* test the quadratic form for practicality */
            seq_t v = base64 + (seq_t)65 * y2;       /* 64·x² + 65·y² */
            seq_t q = floor_sqrt(v);
            if ((q * q) != v) continue;             /* not a perfect square */

            if (!_is_practical_uint64((uint64_t)q))   /* q not practical */
                continue;

            /* Count (z,w) with 0 ≤ z ≤ w, z² + w² = remaining */
            for (idx_t z = 0; (seq_t)z * z <= remaining; ++z) {
                seq_t z2 = (seq_t)z * z;
                seq_t w2 = remaining - z2;
                seq_t w = floor_sqrt(w2);
                if ((w * w) == w2 && z <= w) {
                    ++total;
                }
            }
        }
    }

    return total;
}
