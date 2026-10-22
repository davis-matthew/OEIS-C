/*
 * A342084  Number of chains of distinct strictly superior divisors starting
 *          with n.
 * https://oeis.org/A342084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{d|n, d<n, d>n/d} a(d), a(1)=1
 * Range:     u8 n <= 431, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold results for indices up to n (1‑based). */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    size_t old_cap = cache_cap;
    size_t new_cap = n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();              /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
    /* initialise new entries to zero (or default). */
    for (idx_t i = old_cap; i < new_cap; ++i) {
        cache[i] = 0;
    }
}

/* Forward declaration of the recursive helper. */
static seq_t compute_cached(idx_t n);

/* Enumerate all divisors from the prime factorisation.  When a divisor
   d satisfies d < n and d > n/d, add a(d) to *sum. */
static void
enum_divisors_rec(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                  size_t idx, seq_t current, uint64_t n, seq_t *sum)
{
    if (idx == cnt) {
        if (current < (seq_t)n && current > (seq_t)n / current) {
            *sum += compute_cached((idx_t)current);
        }
        return;
    }
    seq_t val = current;
    const uint64_t p = pr[idx];
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        enum_divisors_rec(pr, ex, cnt, idx + 1, val, n, sum);
        val *= (seq_t)p;
    }
}

static seq_t
compute_cached(idx_t n)
{
    if (n == 0) return 0;            /* undefined except for safety */
    ensure_cache(n);
    seq_t *slot = cache + (n - 1);
    if (*slot != 0) return *slot;    /* already computed */

    if (n == 1) {
        *slot = 1;
        return 1;
    }

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sum = 0;
    enum_divisors_rec(pr, ex, cnt, 0, 1, (uint64_t)n, &sum);

    *slot = 1 + sum;                 /* trivial chain + extensions */
    return *slot;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, avoid UB */
    return compute_cached(n);
}
