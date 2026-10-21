/*
 * A256631  Numbers n such that the decimal expansions of both n and n^2 have
 *          0 as smallest digit and 5 as largest digit.
 * https://oeis.org/A256631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 172, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero_and_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A256631
 *   a(n) = the n-th positive integer n such that the decimal expansions
 *   of n and n^2 have smallest digit 0 and largest digit 5.
 *   Computed by brute‑force search; results are cached for repeated calls.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_zero_and_five.h"

/* Cached sequence numbers.  Indices are 0‑based: cache[0] = a(1). */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;
static size_t cache_cap  = 0;

/* Next integer to test (untyped, keeps all bits for the search). */
static unsigned long long next_test = 1;

/* Main function requested by the problem description. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 */

    /* Initialise / grow cache array. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while ((size_t)n > new_cap) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Find the next valid candidate. */
        while (1) {
            if (has_zero_and_five(next_test)) {
                unsigned long long sq = next_test * next_test;
                if (has_zero_and_five(sq)) {
                    /* Store the candidate.  Cast back to seq_t. */
                    cache[cache_len++] = (seq_t)next_test;
                    ++next_test;
                    break;
                }
            }
            ++next_test;
        }
    }

    return cache[n - 1];
}
