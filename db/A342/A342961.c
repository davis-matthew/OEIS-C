/*
 * A342961  Primes p such that p + the sum of its prime digits is prime.
 * https://oeis.org/A342961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1030, u32 n <= 132, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Computes A342961: primes p such that p + sum of prime digits is also prime. */

static uint64_t prime_digit_sum(uint64_t n)
{
    uint64_t s = 0;
    while (n) {
        unsigned d = (unsigned)(n % 10);
        if (d == 2 || d == 3 || d == 5 || d == 7)
            s += d;
        n /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t next_search = 2;

    /* Ensure we have at least n terms cached. */
    while (len < (size_t)n) {
        uint64_t p = next_prime(next_search);
        next_search = p + 1;

        uint64_t s = prime_digit_sum(p);
        seq_t candidate = (seq_t)(p + s);

        if (is_prime_big(candidate)) {
            if (len + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = alloc_seq_array(new_cap);
                for (size_t i = 0; i < len; ++i)
                    tmp[i] = cache[i];
                free_seq_array(cache);
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
