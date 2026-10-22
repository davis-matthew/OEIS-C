/*
 * A387080  a(1)=1, a(2)=3; thereafter a(n) is either the greatest number k <
 *          a(n-1) not already used such that gcd(k, a(n-1)) > 1, or if no
 *          such k exists then a(n) is the smallest number k > a(n-1) not
 *          already used such that gcd(k, a(n-1)) > 1.
 * https://oeis.org/A387080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=3; for n>2, a(n) is the greatest unused number k < a(n-1) with gcd(k,a(n-1))>1; if none, then a(n) is the smallest unused k > a(n-1) with gcd(k,a(n-1))>1. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* computed sequence values */
static idx_t cache_sz = 0, cache_cap = 0;   /* size and capacity of cache */
static bool *used = NULL;            /* boolean array marking used numbers */
static idx_t used_cap = 0;           /* capacity of used array */
static seq_t last_val = 0;           /* value of the last computed term */

/* Ensure that the used array can index up to indices < need (i.e. size >= need). */
static void ensure_used(idx_t need)
{
    if (need <= used_cap) return;
    idx_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    if (new_cap > used_cap)
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* Ensure that the cache array can hold at least 'need' elements. */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the sequence up to position n (1‑based). */
static void compute_up_to(idx_t n)
{
    if (n <= cache_sz) return;

    /* Initialization on first call. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
        cache_sz = 2;
        last_val = (seq_t)3;

        /* Mark values 1 and 3 as used. */
        ensure_used(4); /* at least indices 0..3 */
        used[1] = true;
        used[3] = true;
    }

    for (idx_t i = cache_sz + 1; i <= n; ++i) {
        seq_t cur = last_val;
        seq_t cand = 0;

        /* Search downwards for the greatest unused k < cur with gcd>1. */
        idx_t cur_idx = (idx_t)cur;
        for (idx_t k = cur_idx - 1; k > 0; --k) {
            if (!used[k] && gcd(cur, (seq_t)k) > (seq_t)1) {
                cand = (seq_t)k;
                break;
            }
        }

        /* If none found, search upwards. */
        if (cand == (seq_t)0) {
            for (idx_t k = cur_idx + 1;; ++k) {
                if (k >= used_cap) ensure_used(k + 1);
                if (!used[k] && gcd(cur, (seq_t)k) > (seq_t)1) {
                    cand = (seq_t)k;
                    break;
                }
            }
        }

        /* Mark the chosen number as used and store it. */
        idx_t ci = (idx_t)cand;
        if (ci >= used_cap) ensure_used(ci + 1);
        used[ci] = true;

        ensure_cache_capacity(cache_sz + 1);
        cache[cache_sz] = cand;
        ++cache_sz;
        last_val = cand;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for n<1, return 0 */
    compute_up_to(n);
    return cache[n - 1];
}
