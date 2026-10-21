/*
 * A277555  Numbers k such that k/5^m == 3 (mod 5), where 5^m is the greatest
 *          power of 5 that divides k.
 * https://oeis.org/A277555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277555: numbers k such that k / 5^v5(k) ≡ 3 (mod 5),
   i.e. k has 3 as its right‑most non‑zero base‑5 digit. */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *cache = NULL;        /* cached values of the sequence   */
static idx_t  cache_len   = 0;     /* how many values are stored      */
static size_t cache_cap   = 0;     /* allocated capacity of the cache */
static seq_t  next_candidate = 1;  /* integer to test next            */

seq_t a(idx_t n)
{
    /* safety – n must be ≥ 1 */
    if (n == 0)
        return 0;

    /* initialise or grow the cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return already computed value */
    if (n <= cache_len) return cache[n-1];

    /* compute missing terms */
    seq_t cand = next_candidate;
    while (cache_len < n) {
        seq_t temp = cand;
        while (temp % 5 == 0) temp /= 5;      /* strip powers of 5 */
        if (temp % 5 == 3)                    /* 3 as right‑most non‑zero digit */
            cache[cache_len++] = cand;
        ++cand;
    }
    next_candidate = cand;

    return cache[n-1];
}
