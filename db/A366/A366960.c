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
 * Validated: standard, pass, 71 terms through n = 9289 (2026-11-03)
 * Helpers:   get_digits, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: numbers whose largest digit minus smallest digit equals 3 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/get_digits.h"
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
