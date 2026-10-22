/*
 * A387231  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest unused
 *          positive number that shares a factor with a(n-1) if a(n-1) >
 *          a(n-2), else it is coprime to a(n-1) if a(n-1) < a(n-2).
 * https://oeis.org/A387231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n) is the smallest unused positive
 *            integer that shares a factor with a(n-1) if a(n-1)>a(n-2),
 *            otherwise it is the smallest unused integer that is coprime to
 *            a(n-1). The sequence is defined recursively with a monotone
 *            search for the next term
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* forward declaration of helper functions */
static void ensure_cache(size_t n);
static void ensure_used(size_t v);

/* static state: memoization cache */
static seq_t *cache = NULL;      /* stores a(1), a(2), ... */
static size_t cache_size = 0;    /* number of values stored */

/* static state: bitmap of used values */
static bool *used = NULL;
static size_t used_cap = 0;      /* current capacity of used[] */

/* enlarge cache to hold at least n slots */
static void ensure_cache(size_t n)
{
    if (cache_size < n) {
        size_t new_cap = cache_size ? cache_size * 2 : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
}

/* enlarge used bitmap to hold index v */
static void ensure_used(size_t v)
{
    if (v >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 128;
        while (v >= new_cap) new_cap <<= 1;
        bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!tmp) abort();
        /* initialise newly added entries to false */
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used = tmp;
        used_cap = new_cap;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* safety */
    /* first call: initialise cache and used bitmap */
    if (!cache) {
        ensure_cache(16);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;
        ensure_used(2);
        used[1] = true;
        used[2] = true;
    }
    /* return from cache if already computed */
    if ((size_t)n <= cache_size) return cache[n-1];

    /* extend computation up to n */
    ensure_cache((size_t)n);
    for (idx_t i = (idx_t)cache_size; i < n; ++i) {
        seq_t prev      = cache[i-1];
        seq_t prevprev  = cache[i-2];
        bool need_share = (prev > prevprev);   /* next term must share a factor? */

        seq_t next_val = 0;
        size_t cand = 1;
        for (;;) {
            ensure_used(cand);
            if (!used[cand]) {
                if (need_share) {
                    if (gcd(prev, (seq_t)cand) != (seq_t)1) { next_val = (seq_t)cand; break; }
                } else {
                    if (gcd(prev, (seq_t)cand) == (seq_t)1) { next_val = (seq_t)cand; break; }
                }
            }
            ++cand;
        }
        cache[i] = next_val;
        ensure_used((size_t)next_val);
        used[(size_t)next_val] = true;
    }
    cache_size = n;
    return cache[n-1];
}
