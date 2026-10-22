/*
 * A394471  Primes of the form prime(k) + prime(k+2) - 1, where prime(k) is
 *          the k-th prime.
 * https://oeis.org/A394471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 740, u32 n <= 741, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394471 – primes of the form prime(k) + prime(k+2) – 1,
   searched by increasing k.  a(1) is the first such prime, a(2) the next,
   and so on.  The function memoises results so that calls with arbitrary
   order of n are handled efficiently. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /*  Static cache of already found terms.  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 1;                 /* next k to test (1‑based) */

    if (n == 0) {                            /* safety for accidental 0 */
        return (seq_t)0;
    }

    /*  Generate further terms until we have at least n of them.  */
    while (cache_len < n) {
        /*  Compute p(k) + p(k+2) – 1  */
        uint64_t pk   = (uint64_t)get_prime(next_k);
        uint64_t pk2  = (uint64_t)get_prime(next_k + 2);
        uint64_t sum  = pk + pk2 - 1;
        seq_t val     = (seq_t)sum;

        /*  Store only if val is itself a prime.  */
        if (is_prime_big(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);      /* graceful if out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_k;                               /* move to next k */
    }

    return cache[n - 1];                         /* a(1) is cache[0] */
}
