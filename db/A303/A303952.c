/*
 * A303952  a(n) is the number of monic polynomials P(z) of degree n over the
 *          complex numbers such that P(z) divides P(z^2).
 * https://oeis.org/A303952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A303952
    a(n) satisfies the linear recurrence
        2*n*a(n)
        +(-13*n+4)*a(n-1)
        + 2*(7*n+3)*a(n-2)
        + 8*(n-7)*a(n-3)
        + 2*(-8*n+33)*a(n-4)
        + 5*(n-4)*a(n-5) = 0
    with initial values
        a(0)=1, a(1)=2, a(2)=5, a(3)=17, a(4)=69
    The function computes a(n) for any n≥0, memoising results.
*/

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t computed_upto = 0;

seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if (n >= cache_size) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL && new_cap > 0) abort();   /* allocation failure */
        cache = tmp;
        cache_size = new_cap;
    }

    /* Initialise the base terms whenever the cache is large enough */
    if (cache_size > 0) {
        cache[0] = 1;
        if (cache_size > 1) cache[1] = 2;
        if (cache_size > 2) cache[2] = 5;
        if (cache_size > 3) cache[3] = 17;
        if (cache_size > 4) cache[4] = 69;
    }

    /* Make sure we have computed at least the base terms */
    idx_t min_base = (cache_size > 4) ? 4 : (idx_t)(cache_size - 1);
    if (computed_upto < min_base) computed_upto = min_base;

    /* Compute successive terms only when they are required */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        /* The recurrence is valid for i >= 5; the loop never
           processes i < 5 because computed_upto is at least 4. */
        assert(i >= 5);

        seq_t pos = 0;
        seq_t neg = 0;

        /* Term for a(i-1): coefficient (13*i-4)  > 0 */
        seq_t coeff = (seq_t)(13 * i - 4);
        pos += coeff * cache[i - 1];

        /* Term for a(i-2): coefficient -2*(7*i+3) < 0 */
        coeff = (seq_t)(2 * (7 * i + 3));
        neg += coeff * cache[i - 2];

        /* Term for a(i-3): coefficient -8*(i-7) */
        idx_t diff = (i > 7) ? (i - 7) : (7 - i);
        coeff = (seq_t)(8 * diff);
        if (i > 7) {
            /* -8*(i-7) is negative for i > 7 */
            neg += coeff * cache[i - 3];
        } else {
            /* -8*(i-7) is positive for i <= 7 */
            pos += coeff * cache[i - 3];
        }

        /* Term for a(i-4): coefficient  2*(8*i-33) */
        idx_t diff4 = (8 * i > 33) ? (8 * i - 33) : (33 - 8 * i);
        coeff = (seq_t)(2 * diff4);
        if (8 * i > 33) {
            pos += coeff * cache[i - 4];
        } else {
            neg += coeff * cache[i - 4];
        }

        /* Term for a(i-5): coefficient -5*(i-4) < 0 */
        coeff = (seq_t)(5 * (i - 4));
        neg += coeff * cache[i - 5];

        /* The numerator is pos - neg; it should be non‑negative. */
        seq_t numerator = (pos >= neg) ? (pos - neg) : (neg - pos);

        /* Divide by 2*i to obtain a(i) */
        seq_t divisor = (seq_t)(2 * i);
        cache[i] = numerator / divisor;
    }

    computed_upto = n;
    return cache[n];
}
