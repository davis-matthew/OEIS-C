/*
 * A389946  Minimum positive value k such that prime(n) - k^2 is a prime
 *          number, or 0 if no such positive k exists.
 * https://oeis.org/A389946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search for the least k with prime(n)-k^2 prime */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static uint64_t *prime_list  = NULL;
static size_t   prime_count  = 0;
static size_t   prime_cap    = 0;
static uint64_t prime_next   = 2;

/* Ensure we have at least n primes stored in prime_list */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    if (!prime_list) {
        prime_cap   = 128;
        prime_list  = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list)
            return;               /* allocation failure – unlikely */
    }

    while (prime_count < (size_t)n) {
        if (is_prime(prime_next)) {
            if (prime_count >= prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp)
                    return;       /* reallocation failure – unlikely */
                prime_list = tmp;
            }
            prime_list[prime_count++] = prime_next;
        }
        if (prime_next == 2)
            prime_next = 3;
        else
            prime_next += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined below offset */

    ensure_primes(n);
    uint64_t p = prime_list[(size_t)n - 1];

    /* Search for the smallest k >= 1 with p - k^2 prime */
    uint64_t limit = (p > 2) ? (p - 2) : 0;
    for (uint64_t k = 1; k * k <= limit; ++k) {
        if (is_prime(p - k * k))
            return (seq_t)k;
    }
    return (seq_t)0;
}
