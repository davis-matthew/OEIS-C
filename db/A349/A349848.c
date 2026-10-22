/*
 * A349848  a(n) = Sum_{k=1..n} prime(n)^prime(k).
 * https://oeis.org/A349848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} prime(n)^prime(k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A349848: a(n) = Σ_{k=1..n} prime(n) ^ prime(k)  (OEIS offset 1). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */
#include "../utils/get_prime.h" /* get_prime(idx_t k) -> uint32_t (1‑indexed) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1; return 0 */

    /* Static cache of the first primes. */
    static uint32_t *prime_cache = NULL;
    static size_t prime_cnt = 0;    /* number of primes currently stored */

    /* Grow the cache if necessary. */
    size_t need = (size_t)n;
    if (prime_cnt < need) {
        uint32_t *new_cache = (uint32_t *)realloc(prime_cache, need * sizeof(uint32_t));
        if (!new_cache) return 0;          /* allocation failure */
        prime_cache = new_cache;
        for (idx_t i = prime_cnt; i < (idx_t)need; ++i) {
            prime_cache[(size_t)i] = get_prime((idx_t)i + 1); /* get_prime is 1‑indexed */
        }
        prime_cnt = need;
    }

    /* prime(n) is the (n‑1)-th index */
    seq_t p = (seq_t)prime_cache[(size_t)(n - 1)];

    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t exp  = (seq_t)prime_cache[(size_t)i];
        seq_t term = big_pow(p, exp);
        sum += term;
    }
    return sum;
}
