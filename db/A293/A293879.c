/*
 * A293879  Numbers having '19' as substring of their digits.
 * https://oeis.org/A293879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose decimal representation
 *            contains the substring "19"
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   has_substring_19
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses bruteforce search, caching previously found terms
   so that non‑increasing calls are fast.  Only integer arithmetic on
   seq_t is used, as required. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_substring_19.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of found terms */
    static idx_t  max    = 0;       /* number of terms stored */
    static size_t cap    = 0;       /* allocated capacity */

    if (n == 0) {                     /* offset is 1, but safeguard */
        return (seq_t)0;
    }

    /* Ensure space for at least n terms. */
    if (n > max) {
        /* Grow array if needed */
        size_t need = (size_t)n;
        if (need > cap) {
            size_t new_cap = cap ? cap << 1 : 16;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – return sentinel */
                return (seq_t)0;
            }
            cache = tmp;
            cap = new_cap;
        }

        /* Find missing terms up to index n */
        seq_t candidate = (max == 0) ? (seq_t)19 : cache[max - 1] + 1;
        for (idx_t i = max + 1; i <= n; ++i) {
            while (!has_substring_19(candidate)) {
                ++candidate;
            }
            cache[i - 1] = candidate;
            ++candidate;
        }
        max = n;
    }

    return cache[n - 1];
}
