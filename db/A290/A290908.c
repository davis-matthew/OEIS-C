/*
 * A290908  p-INVERT of the positive integers, where p(S) = 1 - 4*S^2.
 * https://oeis.org/A290908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 4*a(n-3) - a(n-4) a(0) = 0, a(1) =
 *            4, a(2) = 16, a(3) = 56
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* NULL */

static seq_t *cache = NULL;   /* pointer to the computed values */
static idx_t  cache_cap = 0;  /* total allocated elements */
static idx_t  cache_len = 0;  /* number of terms that have been computed */

/* Ensure the cache can store indices up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failure */
        /* Initialise any new elements to 0 (only needed for POD types) */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 0)
        return 0;                    /* negative indices are not defined */

    ensure_cache(n);

    /* Compute the terms that have not yet been calculated */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = 0;
        else if (i == 1)
            cache[i] = 4;
        else if (i == 2)
            cache[i] = 16;
        else if (i == 3)
            cache[i] = 56;
        else {
            /* Compute a(i) = 4*a(i-1) - 2*a(i-2) + 4*a(i-3) - a(i-4)
               using only unsigned addition and subtraction that are safe. */
            seq_t t1 = cache[i-1];
            t1 *= (seq_t)4;
            seq_t s1 = cache[i-2];
            s1 *= (seq_t)2;
            t1 -= s1;                 /* 4*a(i-1) - 2*a(i-2) (guaranteed non‑negative) */

            seq_t t2 = cache[i-3];
            t2 *= (seq_t)4;
            seq_t s2 = cache[i-4];
            t2 -= s2;                 /* 4*a(i-3) - a(i-4) (guaranteed non‑negative) */

            cache[i] = t1 + t2;       /* final sum */
        }
    }
    cache_len = n + 1;
    return cache[n];
}
