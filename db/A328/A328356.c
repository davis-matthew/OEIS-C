/*
 * A328356  a(n) is the sum of all positive integers whose decimal expansion
 *          is up to n digits and does not contain the 0 digit.
 * https://oeis.org/A328356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (80*90^n - 89*9^n + 9)*5/712
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n > 1: a(n) = 91*a(n-1) - 90*a(n-2) + 45*9^(n-1) */
/* Initial terms: a(0)=0, a(1)=45 */

#include <stdlib.h>  /* for malloc, realloc */

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cache_size = 0;    /* number of terms stored (max index + 1) */
static size_t cache_cap  = 0;    /* allocated capacity of the array */
/* pow9 holds 9^(cache_size-1);  valid after the cache is filled up to non‑empty */
static seq_t pow9 = 0;

seq_t a(idx_t n)
{
    /* integer arithmetic is safe: seq_t is unsigned or arbitrary precision */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)45;

    /* Lazy initialization: ensure cache has space for at least n+1 terms. */
    if (cache == NULL) {
        cache_cap = (size_t) ( (n + 3) > 4 ? (n + 3) : 4 );   /* start big enough */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;                 /* allocation failure */

        /* initialise known terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)45;
        cache_size = 2;
        pow9 = (seq_t)9;          /* 9^(2-1) = 9, for i = 2 next */
    } else if (cache_cap < (size_t)(n + 1)) {      /* enlarge once if needed */
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap = new_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term;
        if (i == 0) {
            term = (seq_t)0;
        } else if (i == 1) {
            term = (seq_t)45;
        } else {
            /* i >= 2: use recurrence and current pow9 = 9^(i-1) */
            term = (seq_t)91 * cache[i - 1]
                 - (seq_t)90 * cache[i - 2]
                 + (seq_t)45 * pow9;
        }
        cache[i] = term;
        /* update pow9 for next iteration */
        pow9 = pow9 * (seq_t)9;
    }

    cache_size = n + 1;      /* now cache[0..n] are valid */
    return cache[n];
}
