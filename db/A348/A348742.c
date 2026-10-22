/*
 * A348742  Odd numbers k for which A161942(k) >= k, where A161942 is the odd
 *          part of sigma.
 * https://oeis.org/A348742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   return the n-th odd k such that the odd part of σ(k) is at least
 *            k
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   append_candidate, odd_part, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"     /* seq_t sigma_of(idx_t n)          */
#include "../utils/odd_part.h"     /* seq_t odd_part(seq_t x)          */
#include "../utils/append_candidate.h" /* void append_candidate(seq_t **p,
                                        size_t *len, size_t *cap, seq_t val) */

static seq_t *cache = NULL;       /* stored terms */
static size_t cache_len = 0;      /* number of stored terms */
static size_t cache_cap = 0;      /* current capacity */

static idx_t next_k = 1;          /* next odd k to test */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0; return 0 for safety */
        return (seq_t)0;
    }

    /* Convert index to size_t for array operations.
       We assume n-1 fits in size_t (true for practical purposes). */
    size_t target = (size_t)(n - 1);

    /* Expand cache until it contains the desired term */
    while (cache_len <= target) {
        seq_t sigma = sigma_of(next_k);          /* σ(k) */
        seq_t odd_sigma = odd_part(sigma);       /* odd part of σ(k) */

        if (odd_sigma >= (seq_t)next_k) {
            /* Store the term */
            append_candidate(&cache, &cache_len, &cache_cap, next_k);
        }

        /* Advance to the next odd integer */
        next_k += 2;
    }

    return cache[target];
}
