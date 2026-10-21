/*
 * A247107  a(0) = 0, a(n) = previous term + repunit of length of previous
 *          term for n > 0.
 * https://oeis.org/A247107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0, a(n) = a(n-1) + repunit of length of a(n-1) for n > 0
 * Range:     u8 n <= 20, u16 n <= 41, u32 n <= 84, u64 n <= 172,
 *            u128 n <= 345, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;   /* array of computed values   */
    static idx_t cached_len  = 0;      /* number of elements stored */
    static seq_t repunit     = 0;      /* repunit for current digit length   */
    static unsigned int cur_digits = 0;/* digit length of the last term */

    /* initialise on first call */
    if (cached_len == 0) {
        cache      = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed -- abort */
        cache[0]   = (seq_t)0;             /* a(0) */
        cached_len = 1;
        repunit    = (seq_t)1;             /* repunit for 1‑digit numbers   */
        cur_digits = 1;                    /* digits(0) = 1 */
    }

    /* grow cache if required */
    if (n + (idx_t)1 > cached_len) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed -- abort */
        cache = tmp;
        /* cached_len remains unchanged until we fill the new slots */
    }

    /* compute missing terms */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t val  = prev + repunit;
        cache[i]   = val;

        /* count decimal digits of val */
        seq_t tmp  = val;
        unsigned int digits = 1;
        while (tmp >= (seq_t)10) {
            tmp /= (seq_t)10;
            ++digits;
        }

        /* if a new digit length is reached, update repunit */
        if (digits > cur_digits) {
            cur_digits = digits;
            repunit    = repunit * (seq_t)10 + (seq_t)1; /* repunit of new length */
        }
    }

    cached_len = (idx_t)(n + 1);
    return cache[n];
}
