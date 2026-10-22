/*
 * A331581  Maximum part of the n-th integer partition in graded
 *          reverse-lexicographic order (A080577); a(1) = 0.
 * https://oeis.org/A331581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = max part of the n‑th partition in graded
 *            reverse‑lexicographic order
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   next_partition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (direct computation, no external utilities used) */

#include <stdlib.h>
#include "../utils/next_partition.h"

static seq_t *cache = NULL;                 /* array of computed values */
static size_t cache_capacity = 0;           /* allocated size of cache    */
static idx_t next_computed_n = 1;           /* next index to compute (1‑based) */

/* state for generating partitions incrementally */
static idx_t cur_sum = 0;                      /* current sum of parts being generated */
static idx_t cur_len = 0;                      /* current partition length */
#define PART_CAP 2000
static seq_t cur_part[PART_CAP];               /* buffer for current partition */

/* ensure the cache can store at least 'size' elements (1‑based indexing) */
static void ensure_cache_capacity(idx_t size) {
    if ((size_t)size <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity : 8;
    while (new_cap < (size_t)size) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);      /* allocation failure */
    cache = new_cache;
    cache_capacity = new_cap;
}

/* generate the next partition of the current sum in reverse‑lexicographic order */

/* public function required by the problem statement */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1, return 0 as a safety */

    ensure_cache_capacity(n + 1);  /* we use 1‑based indexing */

    if (n < next_computed_n) {
        return cache[n];
    }

    /* generate terms until the requested index is reachable */
    while (next_computed_n <= n) {
        if (cur_sum == 0) {      /* handle the empty partition first */
            cache[next_computed_n] = 0;
            cur_sum = 1;
            cur_len = 1;
            cur_part[0] = 1;
        } else {
            cache[next_computed_n] = cur_part[0];  /* maximum part is the first part */
            next_partition(&cur_len, cur_part);
            if (cur_len == 0) {   /* exhausted partitions of current sum */
                cur_sum++;        /* move to the next sum */
                if (cur_sum > PART_CAP) exit(EXIT_FAILURE);
                cur_len = 1;
                cur_part[0] = cur_sum;
            }
        }
        next_computed_n++;
    }

    return cache[n];
}
