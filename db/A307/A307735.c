/*
 * A307735  Integers k such that if m = k + A003132(k) then k = m -
 *          A003132(m).
 * https://oeis.org/A307735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function computes the OEIS sequence A307735.
 * a(n) = the nth integer k (with a(1)=0) such that
 *      if m = k + digit_square_sum(k) then k = m - digit_square_sum(m)
 * which is equivalent to digit_square_sum(k) == digit_square_sum(m).
 *
 * The implementation uses a simple incremental search and memoises
 * previously found terms in a static cache so that calls with
 * non‑sequential n are handled efficiently.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: sum of the squares of the decimal digits of x */
static seq_t digit_square_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        int d = (int)(x % 10);      /* 0 <= d < 10 */
        sum += (seq_t)d * (seq_t)d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache        = NULL;
    static size_t cache_cap    = 0;
    static size_t cache_count  = 0;
    static seq_t last_checked  = 0;   /* k that was last tested */

    /* For n == 0 the sequence is undefined; return 0 as a guard */
    if (n == 0) return 0;

    /* Ensure the first term (0) is stored in the cache */
    if (cache_count == 0) {
        /* Grow cache to at least one element */
        if (cache_cap == 0) {
            cache_cap = 4;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) abort();
        }
        cache[0] = 0;
        cache_count = 1;
        last_checked = 0;
    }

    /* Find enough terms to answer this request */
    while (cache_count < (size_t)n) {
        seq_t k = last_checked + 1;
        seq_t s1 = digit_square_sum(k);
        seq_t m  = k + s1;
        seq_t s2 = digit_square_sum(m);

        if (s1 == s2) {
            /* Grow cache if necessary */
            if (cache_count >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count] = k;
            cache_count++;
        }

        last_checked = k;
    }

    return cache[n - 1];
}
