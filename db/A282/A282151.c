/*
 * A282151  Numbers n with k digits in base x (MSD(n)=d_k, LSD(n)=d_1) such
 *          that, chosen one of their digits in position d_k < j < d_1, is
 *          Sum_{i=j..k}{(i-j+1)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x =
 *          10.
 * https://oeis.org/A282151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer such that a split between two
 *            consecutive * digits yields equal weighted sums (Fulcrum
 *            condition). Implemented by brute‑force * search with memoization
 * Range:     u8 n <= 25, u16 n <= 2813, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_fulcrum_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_fulcrum_number.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* Undefined for 0, return 0 for safety */

    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Grow cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t candidate;
        if (cache_len == 0)
            candidate = (seq_t)1;
        else
            candidate = cache[cache_len - 1] + (seq_t)1;

        /* Find next number satisfying the fulcrum condition. */
        while (!is_fulcrum_number(candidate))
            candidate += (seq_t)1;

        /* Ensure there is room in the cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = candidate;
    }

    return cache[n - 1];
}
