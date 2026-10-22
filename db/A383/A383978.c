/*
 * A383978  Primes with at least two identical trailing digits.
 * https://oeis.org/A383978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   compute OEIS A383978: primes whose last two digits are equal
 * Range:     u8 n <= 4, u16 n <= 641, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine keeps an internal cache of found terms.  Each call
   searches forward until at least n terms have been located.  The
   filter condition is: the number ends with 11 or 33 or 77 or 99
   (these are the only two‑digit patterns that can yield primes > 10). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;
    static uint64_t next = 10;   /* start checking beyond 10 */

    /* ensure we have at least n entries */
    while (cache_len < (size_t)n) {
        uint64_t cand = ++next;
        uint64_t r = cand % 100;
        if (r == 11 || r == 33 || r == 77 || r == 99) {
            if (is_prime(cand)) {
                /* grow cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* out of memory; abort by returning zero */
                        return (seq_t)0;
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cand;
            }
        }
    }
    return cache[(size_t)n - 1];
}
