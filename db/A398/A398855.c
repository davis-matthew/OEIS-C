/*
 * A398855  Upper (1/3, 1)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) Initial
 *            values: a(0)=0, a(1)=2, a(2)=6, a(3)=14, a(4)=26
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* dynamic array holding computed values */
static size_t used  = 0;       /* number of entries actually computed */
static size_t cap   = 0;       /* current allocated capacity */

/* Compute or retrieve a(n) */
seq_t a(idx_t n)
{
    /* We only handle non‑negative indices – the problem guarantees n>=0. */
    size_t idx = (size_t)n;

    /* Initialise cache if needed.  Store the first five terms. */
    if (used == 0) {
        cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* should never happen in tests */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)26;
        used = 5;
    }

    /* Return a known value immediately. */
    if (idx < used)
        return cache[idx];

    /* Grow allocation if necessary. */
    if (cap <= idx) {
        size_t new_cap = idx + 1;
        /* Simple growth strategy – double the size a few times. */
        if (new_cap < cap * 2)
            new_cap = cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* allocation failure – unlikely */
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute new terms up to idx. */
    for (size_t i = used; i <= idx; ++i) {
        /* Recurrence:
           a(i) = 2*a(i-1) - a(i-2) + a(i-3) - 2*a(i-4) + a(i-5)
        */
        seq_t val = 2 * cache[i - 1];
        val = val - cache[i - 2];
        val = val + cache[i - 3];
        val = val - (cache[i - 4] << 1);  /* equivalent to 2*a(i-4) */
        val = val + cache[i - 5];
        cache[i] = val;
    }
    used = idx + 1;
    return cache[idx];
}
