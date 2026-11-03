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
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* ----- helpers for binary representation ----- */
static void get_bits(idx_t x, unsigned char *buf, int *len)
{
    int l = 0;
    idx_t t = x;
    while (t > 0) {
        t >>= 1;
        l++;
    }
    *len = l;
    for (int i = 0; i < l; ++i)
        buf[i] = (x >> (l - 1 - i)) & 1;
}

/* ----- enumerate and count distinct shuffles of binary strings a,b ----- */
static seq_t count_shuffles(idx_t a, idx_t b)
{
    unsigned char bits_a[64], bits_b[64];
    int len_a, len_b;
    get_bits(a, bits_a, &len_a);
    get_bits(b, bits_b, &len_b);
    int L = len_a + len_b;                /* total length of resulting strings */
    if (L == 0) return 1;                 /* unreachable for our n ≥ 1 */

    size_t seen_size = 1u << L;            /* L ≤ 16 for n ≤ 10 000 */
    uint8_t *seen = (uint8_t *)calloc(seen_size, 1u);
    if (!seen) exit(1);                    /* out of memory */
    seq_t cnt = 0;

    for (size_t mask = 0; mask < seen_size; ++mask) {
        if (__builtin_popcount((unsigned)mask) != (unsigned)len_a)
            continue;                     /* mask must contain exactly len_a bits from a */
        uint16_t val = 0;
        int ai = 0, bi = 0;
        for (int pos = 0; pos < L; ++pos) {
            int use_a = (mask >> pos) & 1;
            int bit  = use_a ? bits_a[ai++] : bits_b[bi++];
            val = (val << 1) | (uint16_t)bit;
        }
        if (!seen[val]) {
            seen[val] = 1;
            ++cnt;
        }
    }
    free(seen);
    return cnt;
}

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
            mem_resize(struct cache_entry, cache_arr, new_cap);
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
