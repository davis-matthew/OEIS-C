/*
 * A296866  Numbers whose base-4 digits d(m), d(m-1), ..., d(0) have #(pits) <
 *          #(peaks); see Comments.
 * https://oeis.org/A296866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A296866: a(n) is the n‑th positive integer whose
 * base‑4 digit expansion contains fewer pits than peaks.  */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache of already computed terms */
static seq_t *cache = NULL;     /* pointer to array of seq_t */
static size_t cache_cap = 0;    /* current capacity of the cache     */
static size_t cache_len = 0;    /* number of terms stored in the cache */
static seq_t next_candidate = 1;/* next natural number to test     */

/* Grow the cache to hold at least <need> elements.  */
static void ensure_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        /* realloc returns void*; cast to seq_t* for both C and C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return true if <n> has fewer pits than peaks in its base‑4 representation. */
static bool satisfies(seq_t n)
{
    /* Extract base‑4 digits (most recent digit last). */
    unsigned char digits[128];
    size_t len = 0;
    seq_t x = n;

    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x > 0) {
            digits[len++] = (unsigned char)(x % 4);
            x /= 4;
        }
    }

    int pits = 0;
    int peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        unsigned char a = digits[i - 1];
        unsigned char b = digits[i];
        unsigned char c = digits[i + 1];
        if (a > b && b < c)
            ++pits;
        else if (a < b && b > c)
            ++peaks;
    }
    return pits < peaks;
}

/* Return the nth term of A296866 (1‑based). */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    if (target == 0) return 0;  /* defensive guard, should not happen */

    while (cache_len < target) {
        ensure_cap(cache_len + 1);
        seq_t cand = next_candidate++;
        if (satisfies(cand))
            cache[cache_len++] = cand;
    }
    return cache[target - 1];
}
