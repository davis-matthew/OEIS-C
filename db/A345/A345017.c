/*
 * A345017  Positive even integers with an even number of Goldbach partitions.
 * https://oeis.org/A345017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the nth even integer that has an even number of
   Goldbach partitions.  The function enumerates even numbers, counts
   partitions modulo 2 using a sieve, and stores the indices. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state for memoization */
    static seq_t *seq = NULL;       /* sequence values found */
    static size_t seq_len = 0;      /* number of values stored */
    static size_t seq_cap = 0;      /* allocated capacity */

    static unsigned char *is_prime = NULL;   /* sieve flag array (bytes) */
    static size_t        is_prime_cap = 0;   /* capacity of is_prime array */
    static uint64_t *primes = NULL;          /* list of primes */
    static size_t     primes_cnt = 0;        /* number of primes stored */
    static uint64_t   sieve_limit = 0;       /* maximum value sieved */
    static uint64_t   next_even   = 2;       /* next even number to test */

    /* Ensure we have computed enough terms for the requested index */
    while (seq_len < (size_t)n) {
        /* Extend sieve if necessary */
        if (sieve_limit < next_even) {
            uint64_t new_limit = next_even * 2;
            if (new_limit < next_even) {          /* overflow guard */
                new_limit = UINT64_MAX;
            }

            /* Free old arrays */
            free(is_prime);
            free(primes);

            /* Allocate and initialize new sieve */
            is_prime = (unsigned char*)calloc(new_limit + 1, 1);
            if (!is_prime) return 0;  /* out of memory; should not happen */

            for (uint64_t i = 2; i <= new_limit; ++i)
                is_prime[i] = 1;

            for (uint64_t i = 2; i * i <= new_limit; ++i) {
                if (is_prime[i]) {
                    for (uint64_t j = i * i; j <= new_limit; j += i)
                        is_prime[j] = 0;
                }
            }

            /* Collect primes up to the new limit */
            primes_cnt = 0;
            for (uint64_t i = 2; i <= new_limit; ++i)
                if (is_prime[i]) primes_cnt++;

            primes = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
            if (!primes) return 0;  /* out of memory */

            size_t idx = 0;
            for (uint64_t i = 2; i <= new_limit; ++i)
                if (is_prime[i]) primes[idx++] = i;

            sieve_limit = new_limit;
        }

        /* Count Goldbach partitions for the current even number
           (only parity is needed) */
        bool parity = false;  /* false = even number of partitions */
        uint64_t ev = next_even;
        for (size_t i = 0; i < primes_cnt && primes[i] <= ev / 2; ++i) {
            uint64_t p = primes[i];
            if (is_prime[ev - p]) parity = !parity;
        }

        /* If parity is even, store the value in the sequence */
        if (!parity) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                if (!seq) return 0;  /* out of memory */
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)ev;
        }

        next_even += 2;
    }

    return seq[n - 1];
}
