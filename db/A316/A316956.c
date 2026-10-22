/*
 * A316956  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 5 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A316956.
   Empirical recurrence (order 17):
   a(n) = 10 a(n‑1) + 37 a(n‑2) – 150 a(n‑3) – 869 a(n‑4) – 848 a(n‑5)
          + 778 a(n‑6) + 2809 a(n‑7) + 2206 a(n‑8) – 482 a(n‑9)
          – 2290 a(n‑10) – 1416 a(n‑11) – 190 a(n‑12) + 42 a(n‑13)
          – 23 a(n‑14) + 23 a(n‑15) + 46 a(n‑16)   for n > 17
   Initial values a(1)…a(17) are given below.  The routine memoises
   the sequence so that any order of calls is supported efficiently. */

#include <stddef.h>      /* size_t */
#include <stdlib.h>     /* malloc, free, memcpy */
#include <string.h>     /* memcpy */
#include <stdbool.h>    /* bool */

#include "../utils/seq_from_str.h"   /* seq_t seq_from_str(const char *) */

static seq_t *cache     = NULL;   /* array of computed terms, 0‑based */
static size_t cache_len = 0;      /* number of elements currently stored */
static size_t cache_cap = 0;      /* allocated capacity (in elements) */

/* Decimal representations of a(1)…a(17) */
static const char *const INIT_STR[17] = {
    "8",
    "128",
    "1498",
    "16976",
    "194917",
    "2232443",
    "25592081",
    "293396247",
    "3363774685",
    "38567002402",
    "442190165511",
    "5069971897424",
    "58130384281161",
    "666502012404350",
    "7641876562145390",
    "87619083075260591",
    "1004609897507110225"
};

/* Ensure that cache contains terms up to index n (1‑based). */
static void
ensure_cache(idx_t n)
{
    if (n == 0) return;                      /* not used but keeps contract */

    size_t needed = (size_t)n;               /* number of terms required */

    /* Grow cache if necessary */
    if (!cache || needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first 17 terms lazily */
    if (cache_len < 17) {
        size_t up = 17;
        if (up > needed) up = needed;
        for (size_t i = cache_len; i < up; ++i)
            cache[i] = seq_from_str(INIT_STR[i]);
        if (cache_len < 17) cache_len = 17;
    }

    /* Compute recurrence if we need more than 17 terms */
    if (needed > 17 && cache_len < needed) {
        for (size_t idx = 17; idx < needed; ++idx) {
            /* Build the sum avoiding unsigned negative constants */
            seq_t sum = 0;
            sum += 10 * cache[idx - 1] + 37 * cache[idx - 2];
            sum += 778 * cache[idx - 6] + 2809 * cache[idx - 7] + 2206 * cache[idx - 8];
            sum += 42 * cache[idx - 13] + 23 * cache[idx - 15] + 46 * cache[idx - 16];
            sum -= 150 * cache[idx - 3] + 869 * cache[idx - 4] + 848 * cache[idx - 5];
            sum -= 482 * cache[idx - 9] + 2290 * cache[idx - 10] + 1416 * cache[idx - 11];
            sum -= 190 * cache[idx - 12] + 23 * cache[idx - 14];
            cache[idx] = sum;
        }
        cache_len = needed;
    } else {
        /* Nothing more to compute, just adjust length */
        cache_len = needed;
    }
}

/* Required external function. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not part of the sequence */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
