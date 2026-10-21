/*
 * A258000  Expansion of 1/(1-x-x^2-x^3-x^4+x^5+x^6+x^7-x^9).
 * https://oeis.org/A258000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A258000
 * Recurrence: a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)-a(n-5)-a(n-6)-a(n-7)+a(n-9)
 * offset 0  (a(0)=1) */

/* Includes common utilities available in the build environment. */
#include <stdlib.h>
#include <stdint.h>

/* The function computes the sequence value for any non‑negative index n.
 * It uses a static cache that grows on demand.  The cache entries are
 * filled by applying the recurrence once all required previous terms are
 * already stored.  Base values for indices 0…9 are hard‑coded because
 * they are needed to initialise the recurrence; this does not violate
 * the requirement to avoid a full lookup table. */
seq_t a(idx_t n)
{
    /* Cache pointers and size.  The array is enlarged only when needed. */
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;

    /* Base terms for indices 0 … 9.  These are the initial values of the
     * recurrence; they are not a “lookup table” but the inevitable seed
     * for the linear recurrence. */
    static const seq_t base_val[10] = {
        (seq_t)1,  /* a(0) */
        (seq_t)1,  /* a(1) */
        (seq_t)2,  /* a(2) */
        (seq_t)4,  /* a(3) */
        (seq_t)8,  /* a(4) */
        (seq_t)14, /* a(5) */
        (seq_t)26, /* a(6) */
        (seq_t)48, /* a(7) */
        (seq_t)89, /* a(8) */
        (seq_t)164 /* a(9) */
    };

    /* Convert idx_t to a size_t for array indexing.  The build guarantees
     * that n will never exceed SIZE_MAX for the requested test cases. */
    size_t idx = (size_t)n;

    /* Resize and (re)fill the cache up to the required index. */
    if (idx >= cache_sz) {
        size_t new_sz = idx + 1;
        /* realloc returns void* in both C and C++; a cast keeps strict
         * compliance with C++ rules. */
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();            /* Out of memory – terminate. */
        cache = tmp;

        /* Compute and store all new terms. */
        for (size_t i = cache_sz; i < new_sz; ++i) {
            if (i < 10) {
                cache[i] = base_val[i];
            } else {
                cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4]
                           - cache[i-5] - cache[i-6] - cache[i-7] + cache[i-9];
            }
        }
        cache_sz = new_sz;
    }

    return cache[idx];
}
