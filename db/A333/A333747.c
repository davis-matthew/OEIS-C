/*
 * A333747  Numbers that are either the product of two consecutive primes or
 *          two primes with a prime in between.
 * https://oeis.org/A333747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(ceil(n/2)) * prime(ceil((n+3)/2))
 * Range:     u8 n <= 12, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *primes = NULL;      /* dynamic array of primes */
static size_t primes_count = 0;      /* number of primes stored */
static size_t primes_capacity = 0;   /* capacity of the primes array */
static uint32_t last_candidate = 1;  /* last tested integer */

/* enlarge primes array if needed */
static void ensure_capacity(size_t needed) {
    if (primes_capacity < needed) {
        size_t new_capacity = primes_capacity ? primes_capacity * 2 : 64;
        if (new_capacity < needed)
            new_capacity = needed;
        uint32_t *new_array = (uint32_t *)realloc(primes, new_capacity * sizeof(uint32_t));
        if (!new_array) {
            /* In case of OOM just abort, as thread safety is not a concern here */
            abort();
        }
        primes = new_array;
        primes_capacity = new_capacity;
    }
}

/* trial‑division prime generator, fills primes up to target_count */
static void generate_primes_up_to(size_t target_count) {
    if (primes_count >= target_count)
        return;

    while (primes_count < target_count) {
        last_candidate += 1;
        if (last_candidate == 2) {
            /* 2 is prime */
            ensure_capacity(primes_count + 1);
            primes[primes_count++] = 2;
            continue;
        }
        /* skip even numbers */
        if ((last_candidate & 1u) == 0)
            continue;
        int is_prime = 1;
        for (size_t i = 0; i < primes_count; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > last_candidate)
                break;
            if (last_candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            ensure_capacity(primes_count + 1);
            primes[primes_count++] = last_candidate;
        }
    }
}

/* sequence entry function */
seq_t a(idx_t n) {
    /* n >= 1 is assumed; behaviour for n < 1 is undefined */
    idx_t k1 = (n + 1) / 2;   /* ceil(n/2)   */
    idx_t k2 = (n + 4) / 2;   /* ceil((n+3)/2) */

    idx_t max_k = k1 > k2 ? k1 : k2;
    /* generate enough primes once per call */
    generate_primes_up_to((size_t)max_k);

    /* safe conversion to array indices (k1,k2 are >=1) */
    size_t idx1 = (size_t)(k1 - 1);
    size_t idx2 = (size_t)(k2 - 1);

    seq_t p1 = (seq_t)primes[idx1];
    seq_t p2 = (seq_t)primes[idx2];
    return p1 * p2;
}
