/*
 * A336938  The maximum values of the sets A(n), where A(0) = {0} and A(n+1)
 *          is the union of A(n) and the Collatz orbit of the smallest natural
 *          number missing in A(n).
 * https://oeis.org/A336938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   A(0) = {0}. For n > 0, let m be the smallest natural number not in A(n‑1).  
   Let orbit(m) be its full Collatz orbit (starting at m, following the rules
   3x+1 if odd, x/2 if even, until the value 1 is reached again).  
   A(n) = A(n‑1) ∪ orbit(m).  
   a(n) = max(A(n). */

#include <stdlib.h>
#include <stdint.h>
#ifndef __cplusplus
#  include <stdbool.h>
#endif

/* ---------- internal state ---------- */
static seq_t *cache = NULL;      /* cached a(i) values */
static size_t cache_cap = 0;      /* capacity of cache */
static size_t cache_len = 0;      /* number of elements computed */

static uint8_t *vis = NULL;     /* bitmap: vis[x] != 0 means x has appeared */
static size_t vis_cap = 0;      /* number of bytes allocated in vis */

static uint64_t current_missing = 1;  /* smallest not yet visited */
static uint64_t max_val = 0;          /* current maximum of the set */

/* ---------- helpers for the bitset ---------- */
static void ensure_vis_capacity(uint64_t x)
{
    if (x < vis_cap) return;
    size_t new_cap = vis_cap ? vis_cap * 2 : 1;
    while (new_cap <= (size_t)x) new_cap *= 2;     /* double until big enough */
    uint8_t *tmp = (uint8_t *)realloc(vis, new_cap);
    if (!tmp) abort();
    memset(tmp + vis_cap, 0, new_cap - vis_cap);
    vis = tmp;
    vis_cap = new_cap;
}

static void mark_visited(uint64_t x)
{
    ensure_vis_capacity(x);
    size_t idx = (size_t)x;
    if (!vis[idx]) {
        vis[idx] = 1;
        if (x > max_val) max_val = x;
    }
}

/* ---------- compute a full Collatz orbit ---------- */
static void add_collatz_orbit(uint64_t start)
{
    uint64_t x = start;
    unsigned step = 0;
    while (1) {
        mark_visited(x);
        if (x == 1 && step > 0) break;   /* cycle back to 1 after at least one step */
        if ((x & 1) == 0) {
            x >>= 1;
        } else {
            /* Avoid overflow; abort on impossible values. */
            if (x > (UINT64_MAX - 1) / 3) abort();
            x = 3 * x + 1;
        }
        step++;
    }
}

/* ---------- helpers for the cache ---------- */
static void ensure_cache_capacity(size_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- search for next missing natural number ---------- */
static void find_next_missing(void)
{
    while (1) {
        ensure_vis_capacity(current_missing);
        size_t idx = (size_t)current_missing;
        if (!vis[idx]) break;
        ++current_missing;
    }
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* Initialize first element */
    if (cache_len == 0) {
        ensure_cache_capacity(0);
        cache[0] = (seq_t)0;
        cache_len = 1;

        /* A(0) contains only 0 */
        ensure_vis_capacity(0);
        vis[0] = 1;
        max_val = 0;
    }

    /* Grow cache up to requested index */
    if ((size_t)n >= cache_len) {
        ensure_cache_capacity((size_t)n);
        for (; cache_len <= (size_t)n; ++cache_len) {
            find_next_missing();            /* smallest missing number */
            uint64_t start = current_missing;
            add_collatz_orbit(start);       /* union with its orbit */
            cache[cache_len] = (seq_t)max_val; /* record maximum */
        }
    }

    return cache[(size_t)n];
}
