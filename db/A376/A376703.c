/*
 * A376703  3-brilliant numbers: numbers which are the product of three primes
 *          having the same number of decimal digits.
 * https://oeis.org/A376703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3‑brilliant numbers: products of three primes having the same
   number of decimal digits.  a(n) is the n-th smallest such product
   with offset 1.  The function builds the sequence by generating all
   primes with a given digit length, computing all triple products
   (with repetition), sorting each digit‑length group, and appending the
   sorted values.  Since products of larger digit lengths are
   strictly larger, the final cache is an ordered list.  The function
   memoises results so that any order of calls is handled efficiently. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static seq_t *cache     = NULL; /* memoised sequence values                     */
static size_t cache_len = 0;    /* number of values stored                      */
static size_t cache_cap = 0;    /* allocated capacity of cache                  */
static uint32_t processed_digits = 0; /* highest digit length processed */

/* Return 10^e as an idx_t (e ≥ 0). */
static idx_t pow10_int(uint32_t e)
{
    idx_t r = 1;
    for (uint32_t i = 0; i < e; ++i) r *= 10;
    return r;
}

/* Ensure the cache has enough space for at least `need` additional
   entries.  Grows by a factor of two on overflow. */
static void ensure_cache_capacity(size_t need)
{
    size_t required = cache_len + need;
    if (required <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap < required) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Compare two seq_t values for qsort (ascending order). */
static int cmp_seq_t_local(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Append a sorted array of values to the cache and free the array. */
static void append_sorted_group(seq_t *group, size_t count)
{
    qsort(group, count, sizeof(seq_t), cmp_seq_t_local);
    ensure_cache_capacity(count);
    memcpy(cache + cache_len, group, count * sizeof(seq_t));
    cache_len += count;
    free(group);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, return 0 to avoid UB */

    /* Keep generating digit groups until we have at least n terms. */
    while (cache_len < (size_t)n) {
        ++processed_digits;

        /* Bounds for the current digit length. */
        idx_t lower = pow10_int(processed_digits - 1);
        idx_t upper = lower * 10 - 1;

        /* Generate all primes up to upper. */
        uint64_t *all_primes = NULL;
        size_t all_cnt = 0;
        build_primes_upto(upper, &all_primes, &all_cnt);
        if (all_cnt == 0) { free(all_primes); continue; }

        /* Find index of the first prime with the required digit length. */
        size_t start = 0;
        while (start < all_cnt && all_primes[start] < lower) ++start;
        if (start >= all_cnt) { free(all_primes); continue; }

        /* Count how many primes have this digit length. */
        size_t k = all_cnt - start;          /* number of primes in this group */

        /* Number of triples with repetition: C(k+2, 3). */
        size_t combo_cnt = (((size_t)k + 2) * ((size_t)k + 1) * k) / 6;

        /* Allocate array for this group's products. */
        seq_t *group_vals = (seq_t *)malloc(combo_cnt * sizeof(seq_t));
        if (!group_vals) { free(all_primes); return 0; } /* out of memory */

        /* Generate all i ≤ j ≤ l triples. */
        size_t idx = 0;
        for (size_t a = start; a < all_cnt; ++a) {
            uint64_t p1 = all_primes[a];
            for (size_t b = a; b < all_cnt; ++b) {
                uint64_t p2 = all_primes[b];
                unsigned __int128 mul12 = (unsigned __int128)p1 * p2;
                for (size_t c = b; c < all_cnt; ++c) {
                    uint64_t p3 = all_primes[c];
                    unsigned __int128 prod = mul12 * p3;
                    group_vals[idx++] = (seq_t)prod;
                }
            }
        }
        free(all_primes);

        /* Append the sorted group to the cache. */
        append_sorted_group(group_vals, combo_cnt);
    }

    return cache[n - 1];
}
