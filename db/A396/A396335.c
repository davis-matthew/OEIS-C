/*
 * A396335  Primes of the form k^2 + prime(k)^2.
 * https://oeis.org/A396335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Primes of the form k^2 + prime(k)^2, enumerated by increasing k
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 8, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   append_candidate, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed; for n<1 return 0 (unused). */
    if (n < 1)
        return 0;

    /* Static cache of computed primes of the required form. */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static idx_t   next_k   = 1;            /* next k to test                    */

    /* Continue generating values until we have at least n entries. */
    while (cache_len < (size_t)n)
    {
        /* Compute k^2 + prime(k)^2. */
        seq_t k_sq   = (seq_t)next_k * (seq_t)next_k;
        uint32_t pk  = get_prime(next_k);   /* k‑th prime */
        seq_t pk_sq  = (seq_t)pk * (seq_t)pk;
        seq_t val    = k_sq + pk_sq;

        /* Test primality. */
        if (is_prime_big(val))
            append_candidate(&cache, &cache_len, &cache_cap, val);

        ++next_k;
    }

    return cache[n - 1];
}
