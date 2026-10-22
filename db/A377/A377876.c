/*
 * A377876  The n-th primorial number reduced modulo 27.
 * https://oeis.org/A377876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (product of first n primes) mod 27 with a(0) = 1 (empty
 *            product)
 * Range:     u8 n <= 9720, u16 n <= 9720, u32 n <= 9720, u64 n <= 9720,
 *            u128 n <= 9720, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Cache of primorial mod 27 values */
static seq_t *primorial_cache = NULL;
static size_t primorial_cap   = 0;
static size_t primorial_size  = 0;

/* Cache of primes */
static uint64_t *prime_list = NULL;
static size_t prime_cap    = 0;
static size_t prime_cnt    = 0;

/* Ensure we have at least `needed` primes in prime_list. */
static void ensure_primes(size_t needed)
{
    static uint64_t candidate = 2;
    while (prime_cnt < needed) {
        /* Find next prime */
        for (;;) {
            bool is_prime = true;
            for (size_t j = 0; j < prime_cnt; ++j) {
                uint64_t p = prime_list[j];
                if (p * p > candidate) break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) break;
            ++candidate;
        }

        /* Grow array if necessary */
        if (prime_cap <= prime_cnt) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 1;
            uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            prime_list = tmp;
            prime_cap  = new_cap;
        }
        /* Store the new prime */
        prime_list[prime_cnt++] = candidate;
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /* Initialise cache for a(0) on first call */
    if (primorial_cache == NULL) {
        primorial_cap   = 1;
        primorial_cache = (seq_t*)malloc(primorial_cap * sizeof(seq_t));
        if (!primorial_cache) exit(EXIT_FAILURE);
        primorial_cache[0] = 1;          /* a(0) = 1 */
        primorial_size  = 1;
    }

    /* Build primes up to the required count */
    if (prime_cnt < (size_t)n)
        ensure_primes((size_t)n);

    /* Resize primorial cache if necessary */
    if (primorial_cap <= (size_t)n) {
        size_t new_cap = primorial_cap ? primorial_cap * 2 : 1;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(primorial_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        primorial_cache = tmp;
        primorial_cap   = new_cap;
    }

    /* Compute primorial values up to n */
    for (size_t i = primorial_size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else {
            seq_t prev  = primorial_cache[i - 1];
            seq_t pmod  = (seq_t)(prime_list[i - 1] % 27);
            val = (seq_t)((prev * pmod) % 27);
        }
        primorial_cache[i] = val;
    }
    primorial_size = (size_t)n + 1;

    return primorial_cache[(size_t)n];
}
