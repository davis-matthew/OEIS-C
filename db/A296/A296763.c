/*
 * A296763  Numbers whose base-20 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer such that in base‑20 * its
 *            digit string has more rises than falls. A rise occurs when a *
 *            digit is smaller than its higher‑order neighbour; a fall when it
 *            is * larger. Single‑digit numbers have 0 rises and 0 falls and
 *            therefore * are not included
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily generates terms in ascending order
 * and caches them so repeated calls in any order are efficient. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cached sequence terms */
static idx_t cache_len = 0;          /* number of cached terms */
static idx_t cache_cap = 0;          /* allocated capacity */
static seq_t next_candidate = 1;     /* next integer to test */

/* returns true iff x has more rises than falls in base‑20 representation */
static bool rises_gt_falls(seq_t x)
{
    /* single‑digit numbers never qualify */
    if (x < (seq_t)20) return false;

    /* at most 32 base‑20 digits for 128‑bit numbers */
    seq_t digits[32];
    int len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = tmp % (seq_t)20;
        tmp /= (seq_t)20;
    }

    int rises = 0, falls = 0;
    for (int i = len - 1; i > 0; --i) {
        seq_t hi = digits[i];     /* higher‑order digit */
        seq_t lo = digits[i - 1]; /* lower‑order digit  */
        if (hi < lo) ++rises;
        else if (hi > lo) ++falls;
    }
    return rises > falls;
}

/* ensures the cache has room for at least need more elements */
static void grow_cache(size_t need)
{
    if (cache_cap >= cache_len + need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < cache_len + need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset; harmless */
    /* generate terms until we have at least n terms cached */
    while (cache_len < n) {
        while (!rises_gt_falls(next_candidate)) ++next_candidate;
        grow_cache(1);
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}
