/*
 * A388056  The decimal expansion of the n-th term of A007500, when read
 *          backwards, corresponds to that of the a(n)-th term of A007500.
 * https://oeis.org/A388056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that A007500(k) = A004086(A007500(n)); search over
 *            the reversible primes with memoization and binary search
 * Range:     u8 n <= 84, u16 n <= 11297, u32 n <= 11297, u64 n <= 11297,
 *            u128 n <= 11297, big any n
 * Validated: standard, pass, 82 terms through n = 9878 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint64_t *rp_cache = 0;
static uint64_t rp_len = 0;
static uint64_t rp_cap = 0;
static uint64_t rp_next = 2;

static uint64_t rp_rev(uint64_t n) {
    uint64_t r = 0;
    while (n > 0) {
        r = r * 10 + (n % 10);
        n /= 10;
    }
    return r;
}

static void rp_push(uint64_t v) {
    if (rp_len == rp_cap) {
        rp_cap = rp_cap ? rp_cap * 2 : 1024;
        rp_cache = (uint64_t *)realloc(rp_cache, (size_t)rp_cap * sizeof(uint64_t));
    }
    rp_cache[rp_len++] = v;
}

/* extend until count >= cnt and last term >= minval */
static void rp_extend(uint64_t cnt, uint64_t minval) {
    while (rp_len < cnt || rp_len == 0 || rp_cache[rp_len - 1] < minval) {
        uint64_t c = rp_next++;
        if (is_prime_u64(c) && is_prime_u64(rp_rev(c))) rp_push(c);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    rp_extend((uint64_t)n, 0);
    uint64_t r = rp_rev(rp_cache[n - 1]);
    rp_extend(0, r);
    uint64_t lo = 0, hi = rp_len;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (rp_cache[mid] < r) lo = mid + 1;
        else hi = mid;
    }
    return (seq_t)(lo + 1);
}
