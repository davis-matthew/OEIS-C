/*
 * A393202  a(n) = prime(n + 2)^2 - prime(n + 1)*prime(n).
 * https://oeis.org/A393202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n + 2)^2 - prime(n + 1)*prime(n)
 * Range:     u8 n <= 7, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* cached prime list (1‑based index), using uint32_t because primes < 4 294 967 295
   for n up to 10 000 this is more than enough. */
static uint32_t *prime_cache = NULL;      /* pointer to array of primes      */
static size_t prime_cache_size  = 0;      /* number of primes currently stored */
static size_t prime_cache_cap   = 0;      /* allocated capacity in elements   */
static uint64_t next_candidate  = 2;      /* candidate to test for primality */

/* grow prime_cache to accommodate at least `needed` primes (1‑based). */
static void ensure_primes(idx_t needed)
{
    if (needed == 0) return;
    while (prime_cache_size < (size_t)needed) {
        uint32_t cand = (uint32_t)next_candidate;
        bool isprime = true;

        /* trial division by stored primes up to sqrt(cand) */
        for (size_t i = 0; i < prime_cache_size; ++i) {
            uint32_t p = prime_cache[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }

        if (isprime) {
            /* grow array if necessary */
            if (prime_cache_size >= prime_cache_cap) {
                size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(prime_cache,
                                                    new_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_cache = tmp;
                prime_cache_cap = new_cap;
            }
            prime_cache[prime_cache_size++] = cand;
        }

        ++next_candidate;
        /* skip even numbers after 2 */
        if (next_candidate > 2 && (next_candidate & 1) == 0)
            ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index < 1, return 0 */
    idx_t need = n + 2;            /* need prime(n+2) */
    ensure_primes(need);

    seq_t p_n  = (seq_t)prime_cache[(size_t)n - 1];      /* prime(n)   */
    seq_t p_n1 = (seq_t)prime_cache[(size_t)n];          /* prime(n+1) */
    seq_t p_n2 = (seq_t)prime_cache[(size_t)n + 1];      /* prime(n+2) */

    return p_n2 * p_n2 - p_n1 * p_n;
}
