/*
 * A373392  Inverse binomial transform of A135318.
 * https://oeis.org/A373392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) -5*a(n-2) -2*a(n-3) +2*a(n-4) (n>4) Initial
 *            values: a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=-2
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 28, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;   /* pointer to allocated array */
static size_t cap   = 0;      /* allocated capacity (in elements) */
static size_t size  = 0;      /* number of elements currently stored */

/* ------------------------------------------------------------------ */
/* Allocate initial cache with the first five terms.                 */
/* ------------------------------------------------------------------ */
static void init_cache(void)
{
    static const seq_t init[5] = { 1, 0, 0, 1, -2 };
    cache = (seq_t *)malloc(5 * sizeof(seq_t));
    memcpy(cache, init, 5 * sizeof(seq_t));
    cap  = 5;
    size = 5;
}

/* ------------------------------------------------------------------ */
/* Grow the cache to hold at least (n+1) elements.                    */
/* ------------------------------------------------------------------ */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cap) return;

    /* double capacity until it is enough */
    size_t newcap = cap > 0 ? cap : 16;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (tmp == NULL) abort();           /* out of memory */
    cache = tmp;
    cap   = newcap;
}

/* ------------------------------------------------------------------ */
/* Compute a(n) using the linear recurrence.                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (cache == NULL) init_cache();

    if ((size_t)n < size) return cache[(size_t)n];

    /* Ensure sufficient storage */
    grow_cache(n);

    /* Compute missing terms */
    for (idx_t i = (idx_t)size; i <= n; ++i) {
        seq_t t1 = cache[(size_t)(i - 1)];
        seq_t t2 = cache[(size_t)(i - 2)];
        seq_t t3 = cache[(size_t)(i - 3)];
        seq_t t4 = cache[(size_t)(i - 4)];
        cache[(size_t)i] = -4 * t1 - 5 * t2 - 2 * t3 + 2 * t4;
    }
    size = (size_t)n + 1;
    return cache[(size_t)n];
}
