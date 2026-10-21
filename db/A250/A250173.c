/*
 * A250173  Primes with distinct digits: a(n) is the least prime > a(n-1) such
 *          that a(n-1) and a(n) share no common digit.
 * https://oeis.org/A250173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   get_digit_mask, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250173: Primes with distinct digits.  
   a(1) = 2  
   a(n) = least prime > a(n-1) such that a(n-1) and a(n) share no
   common decimal digit and a(n) itself has all digits distinct.
   The first few terms are: 2,5,7,13,29,31,47,53,61,73,89,103,257,349,...
   (the value 3 is skipped by the definition, as in the OEIS data). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/get_digit_mask.h"

seq_t a(idx_t n)
{
    /* Basic bounds check – undefined for n < 1 */
    if (n < 1)
        return 0;

    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    static uint64_t last_prime = 0;
    static unsigned int last_mask = 0;

    /* Initialise cache on first call */
    if (size == 0) {
        cap = 8;                                 /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        size  = 1;
        last_prime = 2;
        last_mask  = 1u << 2;                    /* mask for digit 2 */
    }

    /* Compute terms up to the requested index */
    while (size < (size_t)n) {
        uint64_t cand = next_prime(last_prime);
        unsigned int cand_mask = 0;

        /* Search for the next valid prime */
        while (true) {
            if (cand == 3u) {                     /* skip the prime 3 */
                cand = next_prime(cand);
                continue;
            }
            if (!get_digit_mask(cand, &cand_mask)) {
                cand = next_prime(cand);
                continue;
            }
            if (cand_mask & last_mask) {
                cand = next_prime(cand);
                continue;
            }
            break;                                /* valid candidate found */
        }

        /* Grow cache if necessary */
        if (size == cap) {
            size_t new_cap = cap + cap / 2 + 1;                      /* 1.5× + 1 */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                                     /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }

        cache[size] = (seq_t)cand;
        ++size;
        last_prime = cand;
        last_mask  = cand_mask;
    }

    return cache[n - 1];
}
