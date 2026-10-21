/*
 * A292489  p-INVERT of the odd positive integers, where p(S) = 1 - S - 6 S^2.
 * https://oeis.org/A292489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - a(n-2) + 15*a(n-3) + 6*a(n-4); initial values:
 *            a(0)=1, a(1)=10, a(2)=60, a(3)=312, a(4)=1656
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of already computed terms   */
static idx_t  cap   = 0;      /* current capacity of cache         */
static idx_t  len   = 0;      /* number of initialized terms       */

/* Ensure the cache can store terms up to 'upto' (inclusive)          */
static void ensure_cache(idx_t upto)
{
    /* allocate or grow the cache if necessary */
    if (cache == NULL) {
        cap = 128;
        while (cap <= upto) cap <<= 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) exit(1);
    } else if (cap <= upto) {
        size_t newcap = cap;
        while (newcap <= (size_t)upto) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (tmp == NULL) exit(1);
        cache = tmp;
        cap = newcap;
    }

    /* initialise base cases if not already done */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)60;
        cache[3] = (seq_t)312;
        cache[4] = (seq_t)1656;
        len = 5;
    }

    /* compute missing terms up to 'upto' */
    for (idx_t i = len; i <= upto; ++i) {
        seq_t term = (seq_t)5 * cache[i-1];
        term -=               cache[i-2];
        term += (seq_t)15 * cache[i-3];
        term += (seq_t)6  * cache[i-4];
        cache[i] = term;
    }
    len = upto + 1;
}

seq_t a(idx_t n)
{
    if (n < 5) {
        switch ((int)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)10;
            case 2: return (seq_t)60;
            case 3: return (seq_t)312;
            case 4: return (seq_t)1656;
        }
    }

    if (n >= len) {
        ensure_cache(n);
    }
    return cache[(size_t)n];
}
