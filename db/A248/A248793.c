/*
 * A248793  Sigma(n) - 1 for n such that sigma(n) - 1 is prime.
 * https://oeis.org/A248793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 7071, u32 n <= 8681, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers n such that sigma(n)-1 is prime; 
   return the corresponding prime value sigma(n)-1. 
   Memoises found primes so that calls in any order run efficiently. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cached prime values sigma(m)-1  */
    static size_t cache_cap = 0;     /* current capacity of cache       */
    static size_t count = 0;         /* number of primes stored         */
    static idx_t  last_checked = 1;  /* last integer examined           */

    /* Ensure at least n values are stored */
    while (count < (size_t)n) {
        ++last_checked;                               /* next candidate   */
        seq_t sigma_val = sigma_of(last_checked);
        seq_t p = sigma_val - (seq_t)1;                /* candidate prime */

        if (p >= (seq_t)2 && is_prime_big(p)) {        /* check primality */
            if (count == cache_cap) {
                /* grow cache; double size or start at 8 */
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                grow_seq(&cache, &cache_cap, new_cap);
            }
            cache[count++] = p;
        }
    }
    return cache[n - 1];
}
