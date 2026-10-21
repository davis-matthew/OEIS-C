/*
 * A283187  a(0) = 0; a(1) = 1; a(2*n) = 2*a(n), a(2*n+1) = a(n) +
 *          (-1)^a(n+1).
 * https://oeis.org/A283187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; a(1) = 1;
   a(2*n)   = 2*a(n);
   a(2*n+1) = a(n) + (-1)^{a(n+1)}  */

/* Note: This implementation uses memoisation to satisfy arbitrary call
   order and keep the runtime linear in the maximum n requested. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>   /* for bool, true, false */

static seq_t *cache   = NULL;  /* computed values */
static bool *filled   = NULL;  /* whether cache[i] is filled */
static idx_t cap      = 0;     /* current capacity of the arrays */

/* Ensure the cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n < cap)
        return;

    idx_t new_cap = n + 1;               /* capacity must be n+1 */

#ifdef __cplusplus
    /* In C++ use new/delete to construct the arrays. */
    seq_t   *new_cache   = new seq_t[new_cap];
    bool    *new_filled  = new bool[new_cap];

    /* Copy any existing data. */
    for (idx_t i = 0; i < cap; ++i) {
        new_cache[i]  = cache[i];
        new_filled[i] = filled[i];
    }
    /* Mark new entries as unfilled. */
    for (idx_t i = cap; i < new_cap; ++i)
        new_filled[i] = false;

    /* Delete old arrays. */
    delete[] cache;
    delete[] filled;

    cache  = new_cache;
    filled = new_filled;
    cap    = new_cap;
#else
    /* In C use realloc; initialise new slots to false. */
    seq_t *new_cache   = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache && new_cap > 0) return;   /* allocation failed, abort silently */

    bool *new_filled  = (bool*)realloc(filled, new_cap * sizeof(bool));
    if (!new_filled && new_cap > 0) {
        /* if realloc for bool fails, rollback the cache realloc */
        free(new_cache);
        return;
    }

    /* Mark new slots as unfilled. */
    for (idx_t i = cap; i < new_cap; ++i)
        new_filled[i] = false;

    cache  = new_cache;
    filled = new_filled;
    cap    = new_cap;
#endif
}

/* Recursive calculation with memoisation. */
static seq_t rec(idx_t n)
{
    ensure_capacity(n);

    if (filled[n])
        return cache[n];

    seq_t result;
    if (n == 0) {
        result = 0;
    } else if (n == 1) {
        result = 1;
    } else if ((n & 1u) == 0u) {               /* even: n = 2 * k */
        result = rec(n >> 1) * 2;
    } else {                                    /* odd: n = 2*k + 1 */
        idx_t k       = n >> 1;
        seq_t a_k     = rec(k);
        seq_t a_k1    = rec(k + 1);             /* a(n+1) */
        /* (-1)^a_k1 is +1 if a_k1 is even, otherwise -1 */
        if ((a_k1 & 1) == 0)
            result = a_k + 1;
        else
            result = a_k - 1;
    }

    cache[n]  = result;
    filled[n] = true;
    return result;
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    return rec(n);
}
