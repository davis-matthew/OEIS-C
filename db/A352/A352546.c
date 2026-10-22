/*
 * A352546  Numbers having more even than odd digits when written in base 10.
 * https://oeis.org/A352546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number having more even than odd decimal digits. */
/* The sequence is defined by brute‑force scanning of natural numbers. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

static seq_t *cache      = NULL;   /* array of computed terms      */
static size_t cache_used = 0;      /* number of terms stored so far */
static size_t cache_cap  = 0;      /* allocated capacity             */
static seq_t last_generated = 0;   /* last integer examined          */

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    cache     = new_ptr;
    cache_cap = new_cap;
}

static void add_value(seq_t val)
{
    ensure_capacity(cache_used + 1);
    cache[cache_used++] = val;
}

/* true if cand has more even decimal digits than odd ones */
static bool qualifies(seq_t cand)
{
    unsigned char cnt[10];
    digit_counts(cand, cnt);
    unsigned int evens = cnt[0] + cnt[2] + cnt[4] + cnt[6] + cnt[8];
    unsigned int odds  = cnt[1] + cnt[3] + cnt[5] + cnt[7] + cnt[9];
    return evens > odds;
}

seq_t a(idx_t n)
{
    /* The sequence is offset 1; a(1) = 0.  Undefined for n < 1. */
    if (n == 0) return (seq_t)0;   /* graceful, though not used */

    /* initialise cache with first term */
    if (cache_used == 0) {
        add_value((seq_t)0);
        last_generated = (seq_t)0;
    }

    /* generate terms until we have at least n of them */
    while (cache_used < (size_t)n) {
        seq_t cand = last_generated + (seq_t)1;
        if (qualifies(cand)) {
            add_value(cand);
        }
        last_generated = cand;
    }
    return cache[n - 1];
}
