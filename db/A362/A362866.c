/*
 * A362866  Numbers k with the property that the parts of the symmetric
 *          representation of sigma(k) are two octagons.
 * https://oeis.org/A362866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A362866: a(n) = 2 * prime(n+2), with prime(1)=2. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Dynamic storage for primes. */
static uint64_t *prime_list = NULL;   /* array of primes, prime_list[0] = 2 */
static size_t   prime_count = 0;      /* number of primes stored */
static size_t   prime_cap   = 0;      /* allocated capacity of prime_list */
static uint64_t last_checked = 1;     /* last number tested for primality */

/* Ensure prime_list contains at least (needed_index+1) primes
   (0‑based index). */
static void ensure_primes_until(size_t needed_index)
{
    if (prime_count > needed_index)
        return;

    if (prime_cap == 0) {
        prime_cap = 4;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    for (;;) {
        uint64_t candidate = ++last_checked;
        if (candidate < 2)
            continue;

        bool is_prime = true;
        /* Test divisibility by known primes up to sqrt(candidate). */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = candidate;
        }

        if (prime_count > needed_index)
            break;
    }
}

/* Return the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    /* We need the (n+2)-th prime, stored at index n+1 (0‑based). */
    size_t idx = (size_t)n + 1;
    ensure_primes_until(idx);
    return (seq_t)prime_list[idx] * (seq_t)2;
}
