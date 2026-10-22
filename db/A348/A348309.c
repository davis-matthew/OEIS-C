/*
 * A348309  a(n) = Sum_{k=0..floor(n/8)} (-1)^k * binomial(n-4*k,4*k).
 * https://oeis.org/A348309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/8)} (-1)^k * binomial(n-4*k, 4*k)
 *            Recurrence: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) -
 *            a(n-8) Initial terms:
 *            a(0)=1,a(1)=1,a(2)=1,a(3)=1,a(4)=1,a(5)=1,a(6)=1,a(7)=1,a(8)=0
 * Range:     i8 n <= 13, i16 n <= 25, i32 n <= 52, i64 n <= 98, i128 n <= 197,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* static cache */
static seq_t *cache = NULL;
static size_t cap   = 0;
static idx_t  max_computed = 0;
static bool   initialized  = false;

static void init_cache(void)
{
    if (initialized) return;
    cap = 16;                     /* start with some capacity */
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) abort();          /* out of memory */

    /* initialise first 9 terms */
    for (idx_t i = 0; i <= 7; ++i)
        cache[i] = (seq_t)1;
    cache[8] = (seq_t)0;
    max_computed = 8;
    initialized = true;
}

static void grow_cache(idx_t needed)
{
    if (needed < cap) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n < cap) {
        if (n <= max_computed) return cache[n];
    }
    /* ensure enough space */
    grow_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 8) {
            cache[i] = (seq_t)0;    /* already 0, but keep for clarity */
        } else {
            /* a(i) = 4*a(i-1) - 6*a(i-2) + 4*a(i-3) - a(i-4) - a(i-8) */
            cache[i] = (seq_t)4 * cache[i-1]
                     - (seq_t)6 * cache[i-2]
                     + (seq_t)4 * cache[i-3]
                     -   cache[i-4]
                     -   cache[i-8];
        }
    }
    max_computed = n;
    return cache[n];
}
