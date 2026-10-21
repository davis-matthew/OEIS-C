/*
 * A243588  Primes p such that sum of digits + 9 is prime.
 * https://oeis.org/A243588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes p such that sum_of_digits(p) + 9 is prime (OEIS A243588)
 * Range:     u8 n <= 24, u16 n <= 2272, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/digit_sum.h"

/* Return the nth term (1‑based) of the sequence.  The function is
   memoised, so multiple calls with arbitrary n are handled in O(n)
   time per call, and the sequence can be queried in any order. */
seq_t a(idx_t n)
{
    /* Static cache of found sequence values. */
    static seq_t *found = NULL;
    static size_t found_len = 0;
    static size_t found_cap = 0;

    /* Cache the last candidate tested, to continue a search efficiently. */
    static seq_t last_candidate = 1;   /* next candidate will start from 2 */

    /* Return the value if it is already cached. */
    if (found_len >= n)
        return found[n - 1];

    /* Ensure the cache array has capacity. */
    if (found_cap == 0) {
        found_cap = 16;
        found = (seq_t *)realloc(found, found_cap * sizeof(seq_t));
        if (!found)
            return 0; /* allocation failed – unlikely in contest environment */
    }

    /* Search for the missing terms. */
    seq_t cand = last_candidate + 1;
    while (found_len < n) {
        /* Check if cand is prime. */
        if (is_prime_small(cand)) {
            /* Check the digit‑sum condition. */
            seq_t s = digit_sum(cand) + (seq_t)9;
            if (is_prime_small(s)) {
                /* Store the term in the cache. */
                if (found_len == found_cap) {
                    found_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(found,
                                                  found_cap * sizeof(seq_t));
                    if (!tmp)
                        return 0; /* allocation failed */
                    found = tmp;
                }
                found[found_len++] = cand;
            }
        }
        last_candidate = cand;
        cand++;
    }

    return found[n - 1];
}
