/*
 * A247867  a(n) is the smallest prime in the interval [k*sqrt(k),
 *          k*sqrt(k+2)], where k = A001359(n), or a(n)=0 if there is no prime
 *          in this interval.
 * https://oeis.org/A247867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime in the interval [k*sqrt(k), k*sqrt(k+2)] *
 *            where k = A001359(n) (the lower member of the n-th twin‐prime
 *            pair). * Returns 0 if no prime lies in that interval
 * Range:     u8 n <= 5, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Integer square root of a 64‑bit unsigned integer: floor(sqrt(n)). */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* Cache of twin‑prime lower members (A001359). */
static uint64_t *twin_lower = NULL;
static size_t twin_count = 0;
static size_t twin_cap   = 0;
static uint64_t last_checked = 2;

/* Extend the cache until it contains at least `n` entries. */
static void extend_twin_primes_until(idx_t n)
{
    for (; twin_count < n; ++twin_count) {
        for (;;) {
            if (last_checked <= 2)
                last_checked = 3;
            else
                last_checked += 2;

            if (is_prime(last_checked) && is_prime(last_checked + 2)) {
                if (twin_count >= twin_cap) {
                    size_t new_cap = twin_cap == 0 ? 8 : twin_cap * 2;
                    uint64_t *tmp = (uint64_t*)realloc(twin_lower, new_cap * sizeof(uint64_t));
                    if (!tmp) exit(1);
                    twin_lower = tmp;
                    twin_cap = new_cap;
                }
                twin_lower[twin_count] = last_checked;
                break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* sequence offset starts at 1 */

    extend_twin_primes_until(n);
    uint64_t k = twin_lower[n - 1];

    /* Bounds: low = ceil(k*sqrt(k)), high = floor(k*sqrt(k+2)). */
    __uint128_t k2       = (__uint128_t)k * k;
    __uint128_t k3       = k2 * k;
    __uint128_t high_val = k2 * (k + 2);

    uint64_t low  = (uint64_t)isqrt_u64((uint64_t)k3) + 1;  /* ceil(k*sqrt(k)) */
    uint64_t high = (uint64_t)isqrt_u64((uint64_t)high_val);/* floor(k*sqrt(k+2)) */

    for (uint64_t p = low; p <= high; ++p) {
        if (is_prime(p))
            return (seq_t)p;
    }
    return (seq_t)0;
}
