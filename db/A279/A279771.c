/*
 * A279771  Numbers n such that the sum of digits of 11n equals 11.
 * https://oeis.org/A279771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Algorithm: a(n) is the n‑th positive integer for which the
   digit sum of 11⋅n equals 11.  The function searches forward
   from the last tested number, storing each qualifying value
   in a static cache so that calls may be in any order. */

/* Compute the sum of decimal digits of x. */
static inline seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != (seq_t)0) {
        sum += x % (seq_t)10;
        x   /= (seq_t)10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* unused for the sequence */

    static seq_t *cache   = NULL;   /* array of found terms   */
    static size_t cache_cap = 0;    /* current allocation size */
    static idx_t  cache_cnt  = 0;   /* number of terms stored */
    static seq_t next_candidate = (seq_t)1; /* first candidate to test */

    /* Allocate or grow the cache if required. */
    if (cache_cnt >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();     /* allocation failure */
        cache   = tmp;
        cache_cap = new_cap;
    }

    seq_t cur = next_candidate;
    while (cache_cnt < n) {
        seq_t prod = cur * (seq_t)11;
        seq_t s    = digit_sum(prod);

        if (s == (seq_t)11)
            cache[cache_cnt++] = cur;

        ++cur;
    }

    next_candidate = cur;   /* advance to the first untested number */

    return cache[n - 1];
}
