/*
 * A296894  Numbers whose base-14 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          = #(peaks); see Comments.
 * https://oeis.org/A296894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th natural number whose base‑14 digits have the
 *            same number of pits and peaks. (A pit is i such that d(i‑1) >
 *            d(i) < d(i+1); a peak is i such that d(i‑1) < d(i) > d(i+1).)
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Test whether a number satisfies the pits=peaks condition in base 14. */
static bool ok(seq_t x)
{
    uint8_t digits[40];
    size_t len = 0;
    seq_t t = x;

    if (t == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (t > 0) {
            digits[len++] = (uint8_t)(t % 14);
            t /= 14;
        }
    }

    int pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        uint8_t a = digits[i - 1];
        uint8_t b = digits[i];
        uint8_t c = digits[i + 1];
        if (a > b && b < c)
            ++pits;
        else if (a < b && b > c)
            ++peaks;
    }
    return pits == peaks;
}

/* Sequence function as required. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;
    static seq_t next = 1;

    /* Expand cache until it contains at least n values. */
    while (cache_sz < (size_t)n) {
        if (cache_sz >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* out of memory – not expected */
            cache = tmp;
            cache_cap = new_cap;
        }
        if (ok(next))
            cache[cache_sz++] = next;
        ++next;
    }
    return cache[n - 1];
}
