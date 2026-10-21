/*
 * A296748  Numbers whose base-12 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296748: numbers whose base‑12 digits have more rises than falls. */
/* Computes terms by scanning natural numbers and caching the results. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cache for the first n terms.  The array is grown as needed. */
static seq_t *cache = NULL;   /* values of the sequence (1‑based index) */
static size_t cache_size = 0; /* number of terms currently cached     */
static size_t cache_cap = 0;  /* allocated capacity of the array       */
static seq_t next_candidate = 1; /* next natural number to test        */

/* Grow the cache to hold at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether a number has more rises than falls in its base‑12 digits. */
static bool good(seq_t x)
{
    seq_t tmp = x;
    int digits[64];
    int len = 0;

    /* Extract digits from least significant to most significant. */
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 12);
        tmp /= 12;
    }
    if (len == 0) return false;        /* not used for zero */

    int rises = 0, falls = 0;
    /* Compare adjacent digits from most significant to least. */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] < digits[i - 1])  ++rises;
        else if (digits[i] > digits[i - 1]) ++falls;
    }
    return rises > falls;
}

/* Extend the cache until it contains at least `n` terms. */
static void extend_cache_to(idx_t n)
{
    if (cache_size >= n) return;
    while (cache_size < n) {
        if (good(next_candidate)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; 0‑th element undefined */
    extend_cache_to(n);
    return cache[n - 1];
}
