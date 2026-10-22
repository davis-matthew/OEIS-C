/*
 * A351377  Primes p such that if the next prime is q, (q-p-1)! mod p is
 *          prime.
 * https://oeis.org/A351377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 333, u32 n <= 202, u64 n <= 10000,
 *            u128 n <= 7459, big any n
 * Helpers:   grow_seq, is_prime_big, mul64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that if next prime is q, then (q-p-1)! mod p is prime. */
/* The function returns the n-th term (offset 1) of the sequence
   A351377. It builds the terms on demand and caches them so that
   subsequent calls are cheap. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/mul64.h"
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 2;

    if (n < 1) return 0;                     /* undefined for n < 1 */

    size_t target = (size_t)n;
    if (cache_len >= target)
        return cache[target - 1];

    /* Generate additional terms until we reach 'n' */
    while (cache_len < target) {
        uint64_t p = next_prime(last_checked);
        uint64_t q = next_prime(p);
        uint64_t m = q - p - 1;              /* gap minus 1 */
        uint64_t factorial_mod = 1;

        /* compute m! mod p */
        for (uint64_t i = 2; i <= m; ++i)
            factorial_mod = mul64(factorial_mod, i, p);

        if (is_prime_big((seq_t)factorial_mod)) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, 1);
            cache[cache_len++] = (seq_t)p;
        }
        last_checked = p;                    /* move to next prime */
    }

    return cache[target - 1];
}
