/*
 * A342083  Number of chains of strictly inferior divisors from n to 1.
 * https://oeis.org/A342083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342083: number of chains of strictly inferior divisors from n to 1
   Formula: a(1) = 1
            a(n) = sum_{d|n, d<sqrt(n)} a(d)   for n>1
          (i.e. sum over all strictly inferior divisors d of n) */
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

#if defined(__cplusplus)
/* allocate and free array of seq_t for C++ */
static inline seq_t* alloc_arr(size_t n) { return new seq_t[n]; }
static inline void free_arr(seq_t* p)    { delete[] p; }
#else
/* allocate and free array of seq_t for C */
#include <stdlib.h>
static inline seq_t* alloc_arr(size_t n) { return (seq_t*)calloc(n, sizeof(seq_t)); }
static inline void free_arr(seq_t* p)    { free(p); }
#endif

/* cache of computed values: a[1..] (index 0 unused) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;    /* size of allocated array */
static size_t last_computed = 0;/* largest n for which cache[n] is valid */

/* recursive enumeration of divisors ≤ sqrt_n */
static void dfs_divs(const uint64_t *pr, const uint64_t *ex,
                     size_t cnt, size_t pos, uint64_t cur,
                     uint64_t sqrt_n, uint64_t n, seq_t *sum)
{
    if (pos == cnt) {
        if ((cur * cur) < n)      /* strictly inferior divisor */
            *sum += cache[cur];
        return;
    }
    uint64_t p = pr[pos];
    uint64_t cur_val = cur;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        dfs_divs(pr, ex, cnt, pos + 1, cur_val, sqrt_n, n, sum);
        if (e == ex[pos]) break;
        if (cur_val > sqrt_n / p) break;  /* next multiplication would exceed sqrt_n */
        cur_val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety: sequence defined for n>=1 */

    /* ensure cache capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;      /* index 0 unused */
        seq_t *new_cache = alloc_arr(new_cap);
        /* copy old values */
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        if (cache) free_arr(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute values up to n if not already done */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        if (i == 1) {                       /* base case */
            cache[1] = (seq_t)1;
            continue;
        }

        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt;

        factor_u64((uint64_t)i, primes, exps, &cnt);

        uint64_t sqrt_n = (uint64_t)floor_sqrt((seq_t)i);
        seq_t sum = (seq_t)0;
        dfs_divs(primes, exps, cnt, 0, 1, sqrt_n, (uint64_t)i, &sum);
        cache[i] = sum;
    }

    last_computed = (size_t)n;
    return cache[n];
}
