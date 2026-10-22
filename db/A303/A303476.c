/*
 * A303476  Square array T(n, k) read by antidiagonals, n > 0 and k > 0: T(n,
 *          k) is the number of distinct shuffles of the words corresponding
 *          to the binary representations of n and of k.
 * https://oeis.org/A303476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) is the number of distinct shuffles of the binary
 *            representations of the pair (i,j) that occupies the nth position
 *            when the infinite square array T(i,j) is read by antidiagonals.
 *            The algorithm stores already‑computed values in a simple cache
 *            and computes each T(i,j) by enumerating all binary...
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   count_shuffles, get_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/count_shuffles.h"
#include "../utils/get_bits.h"

/* ----- simple cache for T(i,j) (i ≤ j) ----- */
struct cache_entry {
    idx_t i;
    idx_t j;
    seq_t val;
};

static struct cache_entry *cache_arr = NULL;
static size_t cache_sz   = 0;
static size_t cache_cap  = 0;

static seq_t get_cache(idx_t a, idx_t b)
{
    if (a > b) { idx_t t = a; a = b; b = t; }
    for (size_t k = 0; k < cache_sz; ++k)
        if (cache_arr[k].i == a && cache_arr[k].j == b)
            return cache_arr[k].val;
    return 0;                               /* 0 means uncached (valid values ≥ 1) */
}

static void add_cache(idx_t a, idx_t b, seq_t val)
{
    if (a > b) { idx_t t = a; a = b; b = t; }
    /* avoid duplicate inserts */
    for (size_t k = 0; k < cache_sz; ++k)
        if (cache_arr[k].i == a && cache_arr[k].j == b)
            return;
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        struct cache_entry *new_arr =
            (struct cache_entry *)realloc(cache_arr, new_cap * sizeof(struct cache_entry));
        if (!new_arr) exit(1);          /* out of memory */
        cache_arr = new_arr;
        cache_cap = new_cap;
    }
    cache_arr[cache_sz].i = a;
    cache_arr[cache_sz].j = b;
    cache_arr[cache_sz].val = val;
    cache_sz++;
}

/* ----- compute T(i,j) with memoisation ----- */
static seq_t T(idx_t i, idx_t j)
{
    if (i == 0 || j == 0) return 0;          /* not used for n ≥ 1 */
    seq_t cached = get_cache(i, j);
    if (cached) return cached;
    seq_t val = count_shuffles(i, j);
    add_cache(i, j, val);
    return val;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, but safe */
    /* find diagonal s such that triangular number order s satisfies
       (s-1)*s/2 < n ≤ s*(s+1)/2 */
    idx_t s = 1;
    while (((s - 1) * s) / 2 < n) ++s;
    idx_t prev_terms = ((s - 2) * (s - 1)) / 2;
    idx_t offset = n - prev_terms - 1;       /* 0‑based position in this diagonal */
    idx_t i = offset + 1;
    idx_t j = s - i;
    return T(i, j);
}
