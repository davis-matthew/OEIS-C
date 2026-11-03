/*
 * A382464  Positive integers that contain an even digit d immediately
 *          preceded by a digit >= d.
 * https://oeis.org/A382464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9685 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer that contains an even digit d
   immediately preceded by a digit p >= d. The function scans
   successive integers, retains those that satisfy the pattern,
   and returns the n-th retained number. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper that tests whether a number has an even digit immediately
   preceded by a digit >= that digit. */
static bool has_even_preceded_by_ge(seq_t x)
{
    if (x < 10)            /* single‑digit numbers cannot satisfy */
        return false;
    while (x >= 10) {
        int d0 = (int)(x % 10);            /* current digit (units)   */
        int d1 = (int)((x / 10) % 10);     /* preceding digit         */
        if ((d0 & 1) == 0 && d1 >= d0)
            return true;
        x /= 10;                           /* move one digit left     */
    }
    return false;
}

/* Main sequence function.  Uses a static dynamic array to cache
   previously computed terms, so calls in any order are efficient. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found terms      */
    static idx_t cache_cap = 0;   /* capacity of cache        */
    static idx_t cache_size = 0;  /* number of terms cached   */
    static idx_t next_candidate = 1;   /* next integer to test    */

    /* Ensure cache storage exists. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Grow cache until we have at least n terms. */
    while (cache_size < n) {
        /* Find next candidate that satisfies the property. */
        while (true) {
            seq_t val = (seq_t)next_candidate;
            if (has_even_preceded_by_ge(val)) {
                /* Store it. */
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_size++] = val;
                next_candidate++;   /* advance to next integer for next search */
                break;
            }
            next_candidate++;       /* check following integer */
        }
    }

    /* Return the requested term. */
    return cache[n - 1];
}
