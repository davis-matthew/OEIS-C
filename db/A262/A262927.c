/*
 * A262927  a(n+9) = a(n) + 10*(n+4) + 9. a(0)=0, a(1)=1, a(2)=3, a(3)=6,
 *          a(4)=10, a(5)=15, a(6)=23, a(7)=30, a(8)=39.
 * https://oeis.org/A262927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n+9) = a(n) + 10*(n+4) + 9 Initial values: a(0)=0, a(1)=1,
 *            a(2)=3, a(3)=6, a(4)=10, a(5)=15, a(6)=23, a(7)=30, a(8)=39
 * Range:     u8 n <= 21, u16 n <= 343, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base_vals[9] = {
    (seq_t)0,
    (seq_t)1,
    (seq_t)3,
    (seq_t)6,
    (seq_t)10,
    (seq_t)15,
    (seq_t)23,
    (seq_t)30,
    (seq_t)39
};

static seq_t *cache = NULL;
static size_t cap   = 0;   /* allocated array size */
static size_t known = 0;   /* highest index computed (-1 if none) */

/* Ensure that cache has space for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* Start with a modest size; will grow as needed. */
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }
    /* Grow capacity until it can hold n+1 entries. */
    while ((idx_t)(cap) <= n) {
        size_t newcap = cap * 2;
        if (newcap == 0) newcap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (tmp == NULL) {
            /* Allocation failed; terminate with an error.
               In production code you might handle this differently. */
            abort();
        }
        cache = tmp;
        cap = newcap;
    }
}

/* Compute a(n).  The function memoizes intermediate values, so
   repeated calls are fast regardless of order. */
seq_t a(idx_t n)
{
    /* Ensure capacity for n. */
    ensure_capacity(n);

    /* If we haven't computed any values yet, initialize known to -1. */
    if (known == 0 && n == 0 && cache[0] == 0 && cache[(size_t)8] == 0)
        known = (size_t)-1;   /* set to -1 so we can compute 0..8 */

    /* Fill in the base values if we haven't done so yet. */
    if (known < 8) {
        size_t start = 0;
        if (known != (size_t)-1) start = known + 1;
        for (size_t i = start; i <= (size_t)8 && i <= (size_t)n; ++i) {
            cache[i] = base_vals[i];
        }
        known = (size_t)8;
    }

    /* Compute any remaining values up to n. */
    for (size_t i = known + 1; i <= (size_t)n; ++i) {
        if (i <= 8) {
            /* Already set by base_vals; skip */
            continue;
        }
        /* Use the recurrence:
           a(i) = a(i-9) + 10*( (i-9)+4 ) + 9
                = a(i-9) + 10*(i-5) + 9
        */
        seq_t term = cache[i - 9];
        term += ((seq_t)10 * ((seq_t)i - (seq_t)5)) + ((seq_t)9);
        cache[i] = term;
    }
    known = (size_t)n;
    return cache[n];
}
