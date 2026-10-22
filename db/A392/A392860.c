/*
 * A392860  a(n) = 1 + Sum_{i=1..n} (p_i - 2)*(p_i#/p_i), where p_i stands for
 *          the i-th prime, and p_i# = A002110(i), the i-th primorial number.
 * https://oeis.org/A392860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 + Sum_{i=1..n} (p_i - 2)*(p_i#/p_i) (p_i# / p_i =
 *            primorial of the first i-1 primes)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache for the sequence, primorials and primes */
    static seq_t *a_cache = NULL;
    static seq_t *prim_cache = NULL;
    static idx_t a_cache_capacity = 0;
    static idx_t max_computed = 0;

    static idx_t *primes = NULL;
    static idx_t prime_capacity = 0;
    static idx_t prime_count = 0;

    /* initialise caches on first call */
    if (a_cache == NULL) {
        a_cache_capacity = 1;
        a_cache = (seq_t*)calloc(a_cache_capacity, sizeof(seq_t));
        prim_cache = (seq_t*)calloc(a_cache_capacity, sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        prim_cache[0] = (seq_t)1;
    }

    /* enlarge sequence cache if necessary */
    if (n >= a_cache_capacity) {
        idx_t new_cap = a_cache_capacity ? a_cache_capacity << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        prim_cache = (seq_t*)realloc(prim_cache, new_cap * sizeof(seq_t));
        a_cache_capacity = new_cap;
    }

    /* compute values up to n if not yet done */
    if (n > max_computed) {
        /* generate any missing primes up to index n */
        for (idx_t i = prime_count; i < n; ++i) {
            idx_t candidate = (prime_count == 0) ? 2 : primes[prime_count - 1] + 1;
            while (!is_prime(candidate)) ++candidate;

            if (prime_count == prime_capacity) {
                idx_t new_cap = prime_capacity ? prime_capacity << 1 : 16;
                primes = (idx_t*)realloc(primes, new_cap * sizeof(idx_t));
                prime_capacity = new_cap;
            }
            primes[prime_count++] = candidate;
        }

        /* compute sequence terms up to n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            idx_t prime = primes[i - 1];                    /* p_i   */
            seq_t p_seq = (seq_t)prime;                    /* cast to seq_t for multiplication */
            seq_t term = (p_seq - (seq_t)2) * prim_cache[i - 1];

            a_cache[i] = a_cache[i - 1] + term;
            prim_cache[i] = prim_cache[i - 1] * p_seq;      /* primorial(i) = primorial(i-1)*p_i */
        }
        max_computed = n;
    }

    return a_cache[n];
}
