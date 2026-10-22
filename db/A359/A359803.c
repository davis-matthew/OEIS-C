/*
 * A359803  a(1) = 1; for n > 1, a(n) = n-1 if a(n-1) = 1, otherwise, apply
 *          the '3x+1' function to a(n-1).
 * https://oeis.org/A359803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A359803:  a(1)=1; for n>1, a(n) = n-1 if a(n-1)=1, else apply  
   the standard 3x+1 map: a(n)=a(n-1)/2 if a(n-1) even, otherwise a(n)=3*a(n-1)+1. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;       /* cache of computed values, indexed from 1 */
static idx_t  capacity = 0;       /* allocated size of cache (largest n for which cache holds data) */
static idx_t  last_computed = 0;  /* highest n that has already been computed */

/* Ensure that the cache can hold at least indices up to 'need'. */
static void ensure_capacity(idx_t need)
{
    if (need <= capacity)
        return;
    idx_t new_cap = need * 2 + 16;          /* grow a bit for future calls */
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache = new_cache;
    capacity = new_cap;
}

/* Return the n-th term of the sequence A359803. */
seq_t a(idx_t n)
{
    if (n == 0)                          /* undefined for n<1, but just in case */
        return (seq_t)0;

    ensure_capacity(n);

    if (last_computed == 0) {            /* first call: initialise a(1) */
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    while (last_computed < n) {
        seq_t prev = cache[last_computed];
        seq_t next;
        if (prev == (seq_t)1) {
            next = (seq_t)(last_computed);          /* a(n) = n-1 when a(n-1)=1 */
        } else if ((prev & 1) == 0) {
            next = prev / 2;                       /* even case */
        } else {
            next = prev * 3 + 1;                    /* odd case */
        }
        cache[++last_computed] = next;
    }

    return cache[n];
}
