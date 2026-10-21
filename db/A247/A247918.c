/*
 * A247918  Expansion of (1 + x) / ((1 - x^4) * (1 + x^4 - x^5)) in powers of
 *          x.
 * https://oeis.org/A247918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 47, i16 n <= 87, i32 n <= 167, i64 n <= 323,
 *            i128 n <= 640, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A247918:
    Expansion of (1 + x) / ((1 - x^4) * (1 + x^4 - x^5))
    Recurrence: a(n) = a(n-5) + a(n-8) - a(n-9)   for n ≥ 9
    Initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=0, a(4)=0,
                    a(5)=1, a(6)=1, a(7)=0, a(8)=1
*/

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t a_cache_len = 0;

static void a_ensure_cache(idx_t n)
{
    /* Allocate or grow cache to at least n+1 elements        */
    if (a_cache == NULL) {
        size_t cap = (size_t)(n + 1);
        if (cap < 9) cap = 9;
        a_cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* initialise fixed first nine terms */
        a_cache[0] = 1; a_cache[1] = 1; a_cache[2] = 0; a_cache[3] = 0; a_cache[4] = 0;
        a_cache[5] = 1; a_cache[6] = 1; a_cache[7] = 0; a_cache[8] = 1;
        a_cache_len = 9;
        a_cache_cap = cap;
    }
    else if ((size_t)(n + 1) > a_cache_cap) {
        size_t new_cap = a_cache_cap * 2;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache_cap = new_cap;
    }

    /* Compute new values if needed */
    if (n >= a_cache_len) {
        for (idx_t i = a_cache_len; i <= n; ++i) {
            a_cache[(size_t)i] =
                a_cache[(size_t)(i - 5)] + a_cache[(size_t)(i - 8)] - a_cache[(size_t)(i - 9)];
        }
        a_cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    a_ensure_cache(n);
    return a_cache[(size_t)n];
}
