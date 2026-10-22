/*
 * A353635  Numbers k such that phi(k) = phi(sigma(k)) and A003958(k) =
 *          A003958(sigma(k)).
 * https://oeis.org/A353635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that phi(k)=phi(sigma(k)) and
 *            A003958(k)=A003958(sigma(k)). They are 1 followed by 2*p where p
 *            is an odd prime >5 with (p+1)/2 also prime
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* utilities used */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static cache for values already computed */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    /* next prime to examine */
    static idx_t cand_prime = 2;

    /* grow cache until we have at least n elements */
    while (cache_len < n) {
        /* first element is 1 */
        if (cache_len == 0) {
            seq_t val = 1;
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cache = newptr;
                cache_cap = newcap;
            }
            cache[cache_len++] = val;
            continue;
        }

        /* find next qualifying prime p */
        while (true) {
            cand_prime++;
            if (!is_prime(cand_prime))
                continue;          /* not a prime */
            if (cand_prime <= 5)   /* skip 2,3,5 */
                continue;
            idx_t q = (cand_prime + 1) >> 1;   /* (p+1)/2 */
            if (!is_prime(q))
                continue;

            seq_t val = (seq_t)cand_prime * (seq_t)2;
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cache = newptr;
                cache_cap = newcap;
            }
            cache[cache_len++] = val;
            break;
        }
    }

    return cache[n - 1];
}
