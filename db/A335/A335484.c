/*
 * A335484  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (3,2,1).
 * https://oeis.org/A335484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   has_decreasing_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th integer k such that the k-th composition
 * (graded reverse-lexicographic order, A066099) contains a strictly
 * decreasing subsequence of length 3 – i.e. the pattern (3,2,1). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_decreasing_triple.h"

/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;
static idx_t next_k = 1;    /* next k to examine */

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 0 */
        return 0;

    /* extend cache until we have at least n entries */
    while (cache_size < n) {
        while (true) {
            if (has_decreasing_triple(next_k)) {
                /* grow cache if necessary */
                if (cache_size == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  cache_cap * sizeof(seq_t));
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)next_k;
                next_k++;
                break;   /* found the next term */
            }
            next_k++;
        }
    }
    return cache[n - 1];
}
