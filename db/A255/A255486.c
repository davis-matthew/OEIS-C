/*
 * A255486  Number of odd terms in expansion of (1+x+x^3+x^4)^n.
 * https://oeis.org/A255486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd coefficients in (1+x+x^3+x^4)^n
 * Range:     u8 n <= 126, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   shift_or_xor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/shift_or_xor.h"

seq_t a(idx_t n)
{
    /* static state ---------------------------------------------------- */
    static seq_t *cache       = NULL;   /* cache of a(k) values       */
    static size_t  cache_size = 0;      /* number of cached entries   */
    static idx_t   last_sub   = (idx_t)-1;   /* last n already computed */

    static uint64_t *poly_cur  = NULL; /* current polynomial bits   */
    static uint64_t *poly_next = NULL; /* temporary polynomial bits */
    static size_t   words = 0;          /* number of 64‑bit words used */

    /* initialization -------------------------------------------------- */
    if (cache_size == 0) {
        /* cache for a(0) */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unlikely, but safe */
        cache[0] = 0;                         /* will be overwritten below */
        cache_size = 1;
        last_sub = 0;

        /* polynomial for (1+x+x^3+x^4)^0  (=1) */
        words = 1;
        poly_cur  = (uint64_t*)calloc(words, sizeof(uint64_t));
        poly_next = (uint64_t*)calloc(words, sizeof(uint64_t));
        if (!poly_cur || !poly_next) return (seq_t)0;
        poly_cur[0] = 1ULL;                  /* coefficient of x^0 is 1 */

        cache[0] = 1;                         /* a(0) = 1 */
        return cache[0];
    }

    /* return cached value -------------------------------------------- */
    if (n <= last_sub)
        return cache[n];

    /* enlarge cache if necessary ------------------------------------- */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* enlarge bitset if necessary ------------------------------------- */
    size_t required_words = (4ULL * (size_t)n) / 64 + 1;
    if (required_words > words) {
        size_t old_words = words;
        words = required_words;
        poly_cur  = (uint64_t*)realloc(poly_cur,  words * sizeof(uint64_t));
        poly_next = (uint64_t*)realloc(poly_next, words * sizeof(uint64_t));
        if (!poly_cur || !poly_next) return (seq_t)0;
        memset(poly_cur + old_words, 0, (words - old_words) * sizeof(uint64_t));
        memset(poly_next + old_words, 0, (words - old_words) * sizeof(uint64_t));
    }

    /* compute values from last_sub+1 up to n -------------------------- */
    for (idx_t k = last_sub + 1; k <= n; ++k) {
        size_t cur_deg  = 4ULL * (size_t)(k - 1);
        size_t cur_words = cur_deg / 64 + 1;
        size_t next_words = 4ULL * (size_t)k / 64 + 1;

        /* clear next polynomial */
        memset(poly_next, 0, words * sizeof(uint64_t));

        /* poly_next = poly_cur ^ (poly_cur<<1) ^ (poly_cur<<3) ^ (poly_cur<<4) */
        shift_or_xor(poly_cur, poly_next, cur_words, words, 0);
        shift_or_xor(poly_cur, poly_next, cur_words, words, 1);
        shift_or_xor(poly_cur, poly_next, cur_words, words, 3);
        shift_or_xor(poly_cur, poly_next, cur_words, words, 4);

        /* count odd coefficients */
        idx_t bits = 0;
        for (size_t i = 0; i < next_words; ++i)
            bits += __builtin_popcountll(poly_next[i]);

        cache[k] = (seq_t)bits;

        /* swap buffers for next iteration */
        uint64_t *tmp = poly_cur;
        poly_cur = poly_next;
        poly_next = tmp;
    }

    last_sub = n;
    return cache[n];
}
