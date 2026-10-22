/*
 * A359031  a(n+1) gives the number of occurrences of the mode of the digits
 *          of a(n) among all the digits of [a(0), a(1), ..., a(n)], with
 *          a(0)=0.
 * https://oeis.org/A359031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 686, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   mode_of_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n≥0,
      a(n+1) = number of occurrences of the mode of the digits of a(n)
      among all the digits of [a(0),a(1),...,a(n)].
   Use the smallest digit when there is a tie. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mode_of_digits.h"

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;     /* capacity of cache array   */
static size_t cache_len = 0;     /* number of values stored   */
static unsigned long long cum_counts[10] = {0};  /* cumulative digit counts */

static inline void init_cache(void) {
    if (cache_len == 0) {
        /* allocate for a(0) */
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;
        cache_len = 1;
        /* cum_counts already zero‑initialized by static storage */
    }
}

static inline void grow_cache(size_t needed) {
    if (cache_cap < needed) {
        /* double capacity for amortised linear growth */
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n < 0) return (seq_t)0;  /* undefined for negative but guard */

    init_cache();

    if (cache_len > (size_t)n)          /* already computed */
        return cache[(size_t)n];

    /* we need to compute up to index n */
    size_t target = (size_t)n;
    grow_cache(target + 1);

    unsigned char digit_buf[128];
    size_t digit_len;

    /* compute values from cache_len up to target */
    for (size_t i = cache_len; i <= target; ++i) {
        seq_t prev = cache[i - 1];

        /* find mode digit of prev */
        unsigned char mode_digit = mode_of_digits(prev, digit_buf, &digit_len);

        /* add prev's digits to cumulative counts */
        for (size_t k = 0; k < digit_len; ++k)
            ++cum_counts[digit_buf[k]];

        /* a(i) is the total count of the mode digit so far */
        cache[i] = (seq_t)cum_counts[mode_digit];
    }

    cache_len = target + 1;
    return cache[(size_t)n];
}
