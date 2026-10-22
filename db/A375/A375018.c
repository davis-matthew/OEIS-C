/*
 * A375018  Numbers k such that repeated application of the Pisano period
 *          eventually gives 24.
 * https://oeis.org/A375018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   compute_pi_prime, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer k such that repeated application
   of the Pisano period eventually yields 24. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_pi_prime.h"
#include "../utils/gcd_u64.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_count = 0;
static idx_t next_to_check = 2;

/* Greatest common divisor for 64‑bit values. */

/* Least common multiple for 64‑bit values. */
static inline uint64_t lcm_u64(uint64_t a, uint64_t b) {
    if (a == 0 || b == 0) return 0;
    return a / gcd_u64(a, b) * b;
}

/* Pisano period for an arbitrary integer n (≤ 2^64).  Combines prime‐power
   formulas and lcm.  Result is guaranteed to fit in 64 bits for all
   n encountered in the sequence up to n ~ 10⁴. */
static seq_t compute_pi_local(idx_t n) {
    if (n <= 1) return (seq_t)1;
    uint64_t m = (uint64_t)n;
    uint64_t result = 1;
    for (uint64_t prime = 2; prime * prime <= m; ++prime) {
        if (m % prime == 0) {
            uint32_t e = 0;
            while (m % prime == 0) { m /= prime; ++e; }
            uint64_t period = 0;
            if (prime == 2) {
                period = 3ULL << (e - 1);
            } else if (prime == 5) {
                uint64_t pow5 = 1;
                for (uint32_t i = 1; i < e; ++i) pow5 *= 5ULL;
                period = 20ULL * pow5;
            } else {
                uint64_t pi_p = compute_pi_prime((idx_t)prime);
                uint64_t powp = 1;
                for (uint32_t i = 1; i < e; ++i) powp *= (uint64_t)prime;
                period = pi_p * powp;
            }
            result = lcm_u64(result, period);
        }
    }
    if (m > 1) {
        uint64_t period;
        if (m == 2) period = 3ULL;
        else if (m == 5) period = 20ULL;
        else period = compute_pi_prime((idx_t)m);
        result = lcm_u64(result, period);
    }
    return (seq_t)result;
}

/* Return true iff repeated application of the Pisano period to x
   eventually reaches 24.  The loop is limited to 400 iterations,
   which is far more than sufficient for the numbers we test. */
static inline bool leads_to_24(idx_t x) {
    idx_t cur = x;
    for (int i = 0; i < 400; ++i) {
        seq_t nxt_seq = compute_pi_local(cur);
        idx_t nxt = (idx_t)nxt_seq;
        if (nxt == 24) return true;
        if (nxt == cur) return false;   /* fixed point other than 24 */
        cur = nxt;
    }
    return false;
}

/* Grow the cache array to hold at least one more element. */
static void ensure_cache(void) {
    if (!cache) {
        cache_size = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
    } else if (cache_count == cache_size) {
        cache_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);
        cache = tmp;
    }
}

/* Main sequence function.  n is 1‑based. */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* behaviour below offset is irrelevant */
    while (cache_count < n) {        /* generate terms until we have enough */
        while (true) {
            if (leads_to_24(next_to_check)) {
                ensure_cache();
                cache[cache_count++] = (seq_t)next_to_check;
                ++next_to_check;
                break;
            }
            ++next_to_check;
        }
    }
    return cache[n - 1];
}
