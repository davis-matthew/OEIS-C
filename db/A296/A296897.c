/*
 * A296897  Numbers whose base-15 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          = #(peaks); see Comments.
 * https://oeis.org/A296897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes A296897 (Numbers in base‑15 with equal pits and peaks). */
/* The function scans integers in increasing order until it has found 'n' numbers
   satisfying the equal‑pits‑and‑peaks condition.  A static cache is kept so
   later calls are fast. */
#include <stdlib.h>
#include <stdbool.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* Return true if x has the same number of pits and peaks in its base‑15 digits. */
static bool balanced(seq_t x)
{
    /* Extract base‑15 digits into a local array (max 128 digits, ample for our range). */
    uint8_t digits[128];
    size_t len = 0;
    if (x == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        seq_t t = x;
        while (t != (seq_t)0 && len < sizeof(digits)) {
            digits[len++] = (uint8_t)(t % (seq_t)15);
            t /= (seq_t)15;
        }
    }

    /* Count pits and peaks. */
    size_t pits = 0, peaks = 0;
    if (len >= 3) {
        for (size_t i = 1; i + 1 < len; ++i) {
            uint8_t a = digits[i - 1];
            uint8_t b = digits[i];
            uint8_t c = digits[i + 1];
            if (a > b && b < c) {
                ++pits;
            } else if (a < b && b > c) {
                ++peaks;
            }
        }
    }
    return pits == peaks;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* offset is 1; for safety */

#if defined(__cplusplus)
    /* C++ implementation – use std::vector as a dynamic array. */
    static std::vector<seq_t> cache;
    static seq_t last_checked = 0;
    while ((idx_t)cache.size() < n) {
        ++last_checked;
        if (balanced(last_checked))
            cache.push_back(last_checked);
    }
    return cache[n - 1];
#else
    /* C implementation – use realloc on a plain array. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;

    while (cache_len < (size_t)n) {
        ++last_checked;
        if (balanced(last_checked)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 32 : cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n - 1];
#endif
}
