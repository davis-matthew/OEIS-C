/*
 * A357675  Smallest m such that A357477(m) = n.
 * https://oeis.org/A357675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that A357477(m) = n, where A357477(m) is
 *            the smallest k with round(sqrt(k*m)) prime; found by direct
 *            search over m
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime_u64.h"

static uint64_t A357675_isqrt(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static bool A357675_hit(uint64_t m, uint64_t k)
{
    uint64_t x = m * k;
    uint64_t r = A357675_isqrt(x);
    uint64_t v = (x - r * r <= r) ? r : r + 1;
    return is_prime_u64(v);
}

seq_t a(idx_t n)
{
    uint64_t m = 1;
    for (;; m++) {
        bool ok = true;
        for (uint64_t k = 1; k < (uint64_t)n; k++) {
            if (A357675_hit(m, k)) { ok = false; break; }
        }
        if (ok && A357675_hit(m, (uint64_t)n))
            return (seq_t)m;
    }
}
