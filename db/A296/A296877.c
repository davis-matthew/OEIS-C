/*
 * A296877  Numbers whose base-8 digits d(m), d(m-1), ..., d(0) have #(pits) >
 *          #(peaks); see Comments.
 * https://oeis.org/A296877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A296877
   Numbers whose base-8 digits d(m), … , d(0) have #(pits) > #(peaks);
   pits: d(i-1) > d(i) < d(i+1)
   peaks: d(i-1) < d(i) > d(i+1)
   The routine returns the nth such number (1‑based indexing).  */
#include <stdlib.h>
#include <stdbool.h>

/* static storage for memoisation */
static seq_t *cache = NULL;   /* array of found numbers */
static size_t cache_len = 0;  /* number of entries stored */
static size_t cache_cap = 0;  /* capacity of the array        */
static idx_t next_to_check = 1; /* next integer to test */

/* Helper to add a new element to the cache, enlarging as needed */
static void add_to_cache(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Count pits and peaks of a number in base 8 */
static bool has_more_pits_than_peaks(seq_t x)
{
    /* extract base‑8 digits into a temporary array */
    int digits[128];
    size_t dlen = 0;
    seq_t tmp = x;
    if (tmp == (seq_t)0) {
        digits[0] = 0;
        dlen = 1;
    } else {
        while (tmp > (seq_t)0) {
            digits[dlen++] = (int)(tmp % (seq_t)8);
            tmp /= (seq_t)8;
        }
    }

    /* count pits and peaks */
    size_t pits = 0, peaks = 0;
    if (dlen >= 3) {
        for (size_t i = 1; i + 1 < dlen; ++i) {
            int left  = digits[i - 1];
            int cur   = digits[i];
            int right = digits[i + 1];
            if (left > cur && right > cur)
                ++pits;
            else if (left < cur && right < cur)
                ++peaks;
        }
    }
    return pits > peaks;
}

seq_t a(idx_t n)
{
    /* Expand cache until we have at least n elements */
    while (cache_len < (size_t)n) {
        seq_t candidate = (seq_t)next_to_check;
        if (has_more_pits_than_peaks(candidate))
            add_to_cache(candidate);
        ++next_to_check;
    }
    return cache[n - 1];
}
