/*
 * A333151  G.f.: Sum_{k>=1} (k^3 * x^(k^2) / Product_{j=1..k} (1 - x^j)).
 * https://oeis.org/A333151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} (k^3 * p_k(n - k^2)), where p_k(m) is the
 *            number of partitions of m with parts not exceeding k. The
 *            function uses incremental DP to keep values up to the largest
 *            requested n
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 172, u64 n <= 686,
 *            u128 n <= 2799, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) without overflow */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Cached partition table: p_k(m) stored row-major for k=0..max_k, m=0..max_m */
static seq_t *partition = NULL;
static idx_t   max_k    = 0;
static idx_t   max_m    = 0;

/* Cached sequence values */
static seq_t *a_cache    = NULL;
static idx_t  a_cache_size = 0;   /* size of a_cache array */
static idx_t  a_cache_max  = (idx_t)-1; /* last computed index */

/* Ensure partition table covers at least all values up to input n */
static void ensure_partitions(idx_t n)
{
    idx_t need_k = isqrt_u64(n);
    idx_t need_m = n;
    if (need_k <= max_k && need_m <= max_m)
        return;               /* already sufficient */

    /* Allocate new table */
    size_t rows = (size_t)(need_k + 1);
    size_t cols = (size_t)(need_m + 1);
    size_t new_size = rows * cols;
    seq_t *new_part = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_part)
        return;   /* allocation failure; leave old table untouched */

    /* Initialize row k = 0 */
    new_part[0] = (seq_t)1;           /* p_0(0) = 1 */
    for (idx_t m = 1; m <= need_m; ++m)
        new_part[m] = (seq_t)0;       /* p_0(m>0) = 0 */

    /* Compute rows k = 1..need_k */
    for (idx_t k = 1; k <= need_k; ++k) {
        size_t base   = (size_t)k * cols;           /* start of row k */
        size_t prev   = (size_t)(k - 1) * cols;     /* start of row k-1 */
        new_part[base] = (seq_t)1;                  /* p_k(0) = 1 */
        for (idx_t m = 1; m <= need_m; ++m) {
            seq_t val = new_part[prev + m];
            if (m >= k)
                val += new_part[base + (m - k)];
            new_part[base + m] = val;
        }
    }

    /* Replace old table */
    if (partition)
        free(partition);
    partition = new_part;
    max_k     = need_k;
    max_m     = need_m;
}

/* Ensure a_cache covers values up to n and compute missing terms */
static void ensure_a_cache(idx_t n)
{
    /* Grow cache array if necessary */
    if (n + 1 > a_cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(a_cache,
                                            new_size * sizeof(seq_t));
        if (!new_cache)
            return; /* allocation failure; keep old array */
        a_cache      = new_cache;
        a_cache_size = new_size;
    }

    /* Initial value a(0) */
    if (a_cache_max == (idx_t)-1) {
        a_cache[0]       = (seq_t)0;
        a_cache_max      = 0;
    }

    /* Compute new terms */
    for (idx_t i = a_cache_max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t kmax = isqrt_u64(i);
        for (idx_t k = 1; k <= kmax; ++k) {
            idx_t t = i - k * k;
            seq_t pk_val = partition[(size_t)k * (max_m + 1) + t];
            seq_t k3 = (seq_t)k * (seq_t)k * (seq_t)k;
            sum += k3 * pk_val;
        }
        a_cache[i] = sum;
    }
    a_cache_max = n;
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    ensure_partitions(n);
    ensure_a_cache(n);
    return a_cache[n];
}
