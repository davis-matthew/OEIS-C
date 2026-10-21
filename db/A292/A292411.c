/*
 * A292411  a(n) = ((prime(n) - 1)/2)^2 modulo prime(n).
 * https://oeis.org/A292411
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((prime(n)-1)/2)^2 (mod prime(n))
 * Range:     u8 n <= 69, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;
static uint64_t prime_cache_next = 1;

static void grow_cache(size_t needed) {
    if (prime_cache_cap >= needed) return;
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t*)realloc(prime_cache, new_cap * sizeof(uint64_t));
    prime_cache = tmp;
    prime_cache_cap = new_cap;
}

static void generate_primes_to(idx_t target) {
    if (prime_cache_len >= (size_t)target) return;
    if (prime_cache_len == 0) {
        grow_cache(16);
        prime_cache[prime_cache_len++] = 2;
        prime_cache_next = 2;
    }
    for (uint64_t cand = prime_cache_next + 1;
         prime_cache_len < (size_t)target; ++cand) {
        if (is_prime(cand)) {
            grow_cache(prime_cache_len + 1);
            prime_cache[prime_cache_len++] = cand;
        }
        prime_cache_next = cand;
    }
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;          /* undefined for offset < 2 */
    generate_primes_to((idx_t)n);
    uint64_t p = prime_cache[n - 1];      /* prime(n) */
    uint64_t half = (p - 1) / 2;
    uint64_t res  = (half * half) % p;    /* (half)^2 mod p */
    return (seq_t)res;
}
