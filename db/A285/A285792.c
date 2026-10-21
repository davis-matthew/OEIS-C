/*
 * A285792  Primes equal to an octagonal number plus 1.
 * https://oeis.org/A285792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes equal to an octagonal number plus 1.
   Computation: generate candidates k*(3*k-2)+1 for k=1,2,3,...,
   test each candidate for primality, and cache the primes found. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* 0‑based array cache: cache[0] = a(1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t tested_k = 0;          /* largest k already tested */

    while (cache_len < n) {
        tested_k++;
        seq_t k = (seq_t)tested_k;
        seq_t candidate = k * ((k * 3) - 2) + 1;   /* k*(3*k-2)+1 */

        if (is_prime_big(candidate)) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = candidate;
        }
    }

    return cache[n - 1];
}
