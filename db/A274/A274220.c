/*
 * A274220  a(n) = (-cos(Pi/7)/cos(2*Pi/7))^n + (-cos(2*Pi/7)/cos(3*Pi/7))^n +
 *          (cos(3*Pi/7)/cos(Pi/7))^n.
 * https://oeis.org/A274220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 20, i64 n <= 42, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3, a(1)=-4, a(2)=10;
   a(n) = -4*a(n-1) - 3*a(n-2) + a(n-3)  (for n ≥ 3) */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i) */
static size_t cache_cap = 0;  /* allocated size of cache */

/* Ensure the cache holds indices up to 'n'.
   Grow the cache if necessary, using realloc. */
static void
ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* indices [0..n] inclusive */
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need * 2; /* overshoot a bit for future growth */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* memory allocation failure */

    /* Initialize newly allocated elements to zero
       (they will be overwritten by the recurrence loop). */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) using the recurrence with memoization. */
seq_t
a(idx_t n)
{
    ensure_cache(n);

    /* Fill cache up to index n if not already present. */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (i < 3 && cache[i] == (seq_t)0) {
            /* Initial terms */
            if (i == 0)
                cache[i] = (seq_t)3;
            else if (i == 1)
                cache[i] = (seq_t)-4;
            else /* i == 2 */
                cache[i] = (seq_t)10;
            continue;
        }

        if (i >= 3 && cache[i] == (seq_t)0) {
            /* Use recurrence: a(i) = -4*a(i-1) - 3*a(i-2) + a(i-3) */
            cache[i] = ((seq_t)-4) * cache[i - 1]
                     + ((seq_t)-3) * cache[i - 2]
                     + cache[i - 3];
        }
    }

    return cache[(size_t)n];
}
