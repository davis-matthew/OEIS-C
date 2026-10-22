/*
 * A334992  a(n) is the smallest number larger than a(n-1) whose a(i)-th
 *          binary digit is 0 for all i<n, with a(1)=1.
 * https://oeis.org/A334992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest number > a(n-1) whose a(i)-th binary digit is 0
 *            for all i<n, a(1)=1
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 9921, u64 n <= 9921,
 *            u128 n <= 9921, big any n
 * Helpers:   has_forbidden_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/has_forbidden_bit.h"

seq_t a(idx_t n) {
    /* Persistent cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (excluding index 0) */
    static size_t cache_cap = 0;   /* allocate capacity */

    /* Table of forbidden positions (bit indices that may not be 1) */
    static char *forbidden = NULL;
    static size_t forb_cap = 0;    /* capacity of forbidden array */

    if (n < 1) return (seq_t)0;   /* undefined for n less than offset */

    /* Initialisation on first call */
    if (cache_len == 0) {
        /* start with a(1) = 1 */
        cache_cap = 2;                    /* indices 0 and 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)1;
        cache_len = 1;

        /* initialise forbidden positions; initially only position 1 is forbidden */
        forb_cap = 2;                     /* indices 0 and 1 */
        forbidden = (char *)calloc(forb_cap, sizeof(char));
        forbidden[1] = 1;                 /* a(1) = 1 forbids bit 1 */
    }

    /* If we already know a(n), return it */
    if ((idx_t)n <= cache_len) {
        return cache[n];
    }

    /* Grow cache arrays up to n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap <= (size_t)n) new_cap = n * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute required terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Start looking for the next allowed number */
        seq_t x = cache[i - 1] + (seq_t)1;
        while (has_forbidden_bit(x, forbidden, forb_cap)) {
            x += (seq_t)1;   /* try next integer */
        }
        cache[i] = x;

        /* Mark the bit position equal to this new term as forbidden for later terms */
        size_t posX = (size_t)x;
        if (posX >= forb_cap) {
            size_t new_cap = forb_cap ? forb_cap : 1;
            while (new_cap <= posX) new_cap <<= 1;
            forbidden = (char *)realloc(forbidden, new_cap * sizeof(char));
            /* initialise the newly added entries to 0 */
            memset(forbidden + forb_cap, 0, (new_cap - forb_cap) * sizeof(char));
            forb_cap = new_cap;
        }
        forbidden[posX] = 1;
    }

    cache_len = (size_t)n;
    return cache[n];
}
