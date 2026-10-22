/*
 * A320432  Expansion of e.g.f. exp(3 * (1 - exp(x)) + x).
 * https://oeis.org/A320432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A320432:  a(0) = 1; for n > 0
   a(n) = a(n-1) - 3 * Sum_{k=0}^{n-1} binomial(n-1,k) * a(k)
   where binomial(m,k) = m!/(k!(m-k)!) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* cached values of the sequence */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;
static idx_t a_cache_last = 0;

/* ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (a_cache_size == 0) {
        a_cache_size = n + 1;
        a_cache = (seq_t *)malloc(a_cache_size * sizeof(seq_t));
    } else if (a_cache_size <= (size_t)n) {
        size_t new_size = a_cache_size * 2;
        if (new_size <= (size_t)n) new_size = n + 1;
        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        memcpy(tmp, a_cache, a_cache_size * sizeof(seq_t));
        free(a_cache);
        a_cache = tmp;
        a_cache_size = new_size;
    }
    /* initialise a(0) if not yet set */
    if (a_cache_last == 0 && n >= 0 && a_cache[0] == 0) {
        a_cache[0] = (seq_t)1;
    }
}

/* main function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n <= a_cache_last) return a_cache[n];

    for (idx_t i = a_cache_last + 1; i <= n; ++i) {
        /* compute sum_{k=0}^{i-1} binomial(i-1,k) * a(k) */
        seq_t sum = 0;
        seq_t binom = 1;          /* binomial(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            seq_t prod = binom * a_cache[k];
            sum += prod;
            if (k + 1 < i) {
                /* update binomial to binomial(i-1,k+1) */
                binom = binom * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
            }
        }
        /* a(i) = a(i-1) - 3 * sum */
        a_cache[i] = a_cache[i - 1] - (seq_t)3 * sum;
    }
    a_cache_last = n;
    return a_cache[n];
}
