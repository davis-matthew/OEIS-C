/*
 * A366960  Numbers whose difference between the largest and smallest digits
 *          is equal to 3.
 * https://oeis.org/A366960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 3426, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: numbers whose largest digit minus smallest digit equals 3 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static seq_t  cur = 10;          /* first 2‑digit number to examine */
    static bool   initialized = false;

    if (!initialized) {
        initialized = true;
        cache_len = 0;
        cache_cap = 0;
        cur = 10;   /* implicit integer conversion is assumed to work */
    }

    /* Find terms until we reach the requested index */
    while (cache_len < (size_t)n) {
        /* Get digits of cur */
        uint8_t buf[256];
        size_t   dlen = 0;
        get_digits(cur, buf, &dlen);

        uint8_t min_d = 9, max_d = 0;
        for (size_t i = 0; i < dlen; ++i) {
            uint8_t d = buf[i];
            if (d < min_d) min_d = d;
            if (d > max_d) max_d = d;
        }

        if (max_d - min_d == 3) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();          /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;      /* store the term */
        }
        ++cur;                          /* next candidate */
    }

    /* Return the term indexed by n (1‑based) */
    return cache[n - 1];
}
