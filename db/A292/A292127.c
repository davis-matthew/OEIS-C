/*
 * A292127  a(1) = 1, a(r(n)^k) = 1 + k * a(n) where r(n) is the n-th number
 *          that is not a perfect power A007916(n).
 * https://oeis.org/A292127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + k * a(d) where n = r(d)^k and r(d) is the d‑th
 *            non‑perfect‑power number
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   pow_eq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_eq.h"

/* ----- integer sqrt for idx_t ------------------------------------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid) {
            hi = mid - 1;
        } else {
            lo = mid;
        }
    }
    return lo;
}

/* ----- checks for x^k <= n and x^k == n, safe from overflow -------- */
static bool pow_leq(idx_t x, idx_t k, idx_t n)
{
    idx_t prod = 1;
    for (idx_t i = 0; i < k; ++i) {
        if (prod > n / x) return false;
        prod *= x;
    }
    return prod <= n;
}

/* ----- floor kth root of n ---------------------------------------- */
static idx_t kth_root_floor(idx_t n, idx_t k)
{
    if (k == 1) return n;
    idx_t lo = 1, hi = isqrt_u64(n);
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (pow_leq(mid, k, n)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

/* ----- mapping number -> index among non‑perfect powers ------------- */
static idx_t *num_to_index = NULL;
static idx_t max_map_limit = 0;

static void ensure_map_upto(idx_t limit)
{
    if (limit <= max_map_limit) return;

    idx_t new_size = limit + 1;
    idx_t *new_map = (idx_t*)malloc(new_size * sizeof(idx_t));
    if (!new_map) abort();

    bool *perfect = (bool*)calloc(new_size, sizeof(bool));
    if (!perfect) abort();

    for (idx_t base = 2; base * base <= limit; ++base) {
        idx_t val = base * base;
        while (val <= limit) {
            perfect[(size_t)val] = true;
            if (val > limit / base) break;
            val *= base;
        }
    }

    idx_t cnt = 0;
    for (idx_t i = 2; i < new_size; ++i) {
        if (!perfect[(size_t)i]) {
            ++cnt;
            new_map[(size_t)i] = cnt;   /* r(cnt) = i */
        } else {
            new_map[(size_t)i] = 0;
        }
    }
    new_map[1] = 0;   /* unused */

    free(num_to_index);
    free(perfect);
    num_to_index = new_map;
    max_map_limit = limit;
}

/* ----- cache for a(n) values -------------------------------------- */
static seq_t *a_cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache_upto(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = n + 1;
    seq_t *new_cache = a_cache ? (seq_t*)realloc(a_cache, new_size * sizeof(seq_t))
                               : (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_cache) abort();

    if (!a_cache) {
        for (idx_t i = 0; i < new_size; ++i) new_cache[i] = 0;
    } else if (new_size > cache_size) {
        for (idx_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
    }

    a_cache = new_cache;
    cache_size = new_size;
}

/* ----- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    ensure_map_upto(n);
    ensure_cache_upto(n);

    if (a_cache[n] != 0) return a_cache[n];

    if (n == 1) {
        a_cache[1] = 1;
        return 1;
    }

    /* find maximal exponent k such that n = base^k with base non‑perfect‑power */
    idx_t k = 1;
    idx_t base = n;

    for (idx_t exp = 64; exp >= 2; --exp) {
        if (!pow_leq(2, exp, n)) continue;   /* 2^exp > n, skip */
        idx_t r = kth_root_floor(n, exp);
        if (r > 1 && pow_eq(r, exp, n)) {
            if (num_to_index[r] != 0) {     /* r is not a perfect power */
                k = exp;
                base = r;
                break;
            }
        }
    }

    idx_t d = num_to_index[base];
    if (d == 0) d = 1;   /* safety, should not occur */

    seq_t val = a(d);
    seq_t result = 1 + (seq_t)k * val;

    a_cache[n] = result;
    return result;
}
