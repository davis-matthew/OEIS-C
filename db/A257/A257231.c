/*
 * A257231  a(n) = n^2 mod p where p is the least prime greater than n.
 * https://oeis.org/A257231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod p where p is the least prime greater than n
 * Range:     u8 n <= 522, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

static uint64_t *prime_cache = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;

static void add_prime(uint64_t p)
{
    if (prime_count >= prime_capacity) {
        size_t new_cap = prime_capacity ? (prime_capacity << 1) : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        prime_cache = tmp;
        prime_capacity = new_cap;
    }
    prime_cache[prime_count++] = p;
}

static uint64_t next_prime_uint64(uint64_t n)
{
    uint64_t candidate = n + 1;
    while (1) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_cache[i] * prime_cache[i] <= candidate; ++i) {
            if (candidate % prime_cache[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            add_prime(candidate);
            return candidate;
        }
        /* safety: avoid infinite loop if candidate overflows */
        if (candidate == UINT64_MAX) {
            return UINT64_MAX;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }
    uint64_t p = next_prime_uint64((uint64_t)n);
    uint64_t res = mul_mod64((uint64_t)n, (uint64_t)n, p);
    return (seq_t)res;
}
