/*
 * A341003  Numbers whose sum of even digits and sum of odd digits differ by
 *          2.
 * https://oeis.org/A341003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4975, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A341003: numbers n such that
   |(sum of even digits) – (sum of odd digits)| = 2. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether a number has the required property. */
static bool good(seq_t x)
{
    seq_t se = 0;   /* sum of even digits   */
    seq_t so = 0;   /* sum of odd  digits   */
    seq_t tmp = x;

    while (tmp > 0) {
        uint8_t d = (uint8_t)(tmp % (seq_t)10);   /* 0 .. 9 */
        if (d & 1)
            so += d;
        else
            se += d;
        tmp /= (seq_t)10;
    }

    seq_t diff = se > so ? se - so : so - se;
    return diff == (seq_t)2;
}

/* ------------------------------------------------------------------ */
/*   Cached search for the sequence terms                               */
/* ------------------------------------------------------------------ */

static seq_t  *cache          = NULL;      /* array of found terms      */
static size_t  cache_cap     = 0;          /* allocated capacity        */
static size_t  cache_size    = 0;          /* number of terms stored    */
static uint64_t next_scan    = 1;          /* next candidate to examine */

/* Ensure that the cached array holds at least `need` terms. */
static void ensure_cache(size_t need)
{
    if (cache_size >= need)
        return;

    /* Grow capacity if necessary. */
    if (cache_cap < need) {
        if (cache_cap == 0)
            cache_cap = 8;
        while (cache_cap < need)
            cache_cap <<= 1;                /* double capacity */

        seq_t *newptr = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!newptr) {
            /* Allocation failed – abort rather than crash. */
            abort();
        }
        cache = newptr;
    }

    /* Extend the cache until we have enough terms. */
    while (cache_size < need) {
        seq_t val = (seq_t)next_scan;     /* cast the scanner value */
        if (good(val)) {
            cache[cache_size++] = val;
        }
        ++next_scan;
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Convert the index to a C size_t.  The caller guarantees n >= 1. */
    size_t idx = (size_t)n;

    ensure_cache(idx);
    return cache[idx - 1];
}
