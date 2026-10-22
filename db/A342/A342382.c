/*
 * A342382  a(0) = 0; for n > 0, a(n) is the least positive integer not
 *          occurring earlier such that both the digits in a(n) and the digits
 *          in a(n-1)*a(n) are all distinct.
 * https://oeis.org/A342382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342382:
   a(0) = 0.
   For n > 0, a(n) is the least positive integer not occurring earlier
   such that all digits of a(n) are distinct and all digits of a(n‑1)*a(n)
   are distinct.  The function memoises the sequence for any order of
   calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Return true iff the decimal representation of x has all distinct digits. */
static bool digits_distinct(seq_t x)
{
    unsigned mask = 0;
    if (x == 0) return true;          /* zero has a single digit 0 */
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        if (mask & (1U << d)) return false;
        mask |= 1U << d;
        x /= 10;
    }
    return true;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Static state: memoised values and used-number bitset. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t computed   = 1;     /* number of terms already computed (>=1) */

    static unsigned char *used   = NULL;
    static size_t used_cap       = 0;

    /* Initialise on first call. */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = 0;                  /* a(0) = 0 */
    }

    if (used_cap == 0) {
        used_cap = 1000000;            /* initially 1 million entries */
        used = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
        used[0] = 1;                  /* 0 is already used */
    }

    /* Expand the cache array if we need to store more terms. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms up to n if necessary. */
    while (computed <= (size_t)n) {
        seq_t prev = cache[computed - 1];
        seq_t cand = 1;

        /* Search for the next valid candidate. */
        for (;;) {
            size_t idx = (size_t)cand;

            /* Grow the used bitset if required. */
            if (idx >= used_cap) {
                size_t new_cap = used_cap;
                while (new_cap <= idx) new_cap <<= 1;
                unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
                /* Zero the newly allocated part. */
                memset(tmp + used_cap, 0, new_cap - used_cap);
                used = tmp;
                used_cap = new_cap;
            }

            if (used[idx]) {             /* number already used */
                ++cand;
                continue;
            }
            if (!digits_distinct(cand)) { /* candidate's digits not distinct */
                ++cand;
                continue;
            }

            seq_t prod = prev * cand;     /* compute product in seq_t type */
            if (!digits_distinct(prod))  /* product's digits not distinct */
                ++cand;
            else
                break;                   /* found valid candidate */
        }

        /* Store it and mark as used. */
        cache[computed] = cand;
        used[(size_t)cand] = 1;
        ++computed;
    }

    return cache[n];
}
