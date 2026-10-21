/*
 * A270828  a(n) = (Sum_{k=1..2n-1} prime(k)) mod prime(n).
 * https://oeis.org/A270828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of first (2*n-1) primes) mod nth prime
 * Range:     u8 n <= 7, u16 n <= 190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation dynamically builds the prime list and their prefix
   sums on demand.  The arrays are stored in static variables so that calls
   are efficient irrespective of call order.  All arithmetic is performed in
   type seq_t; the mod operator works for the arbitrary‑precision build. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 to keep behaviour defined. */
    if (n == 0) return 0;

    /* Static caches for primes and prefix sums. */
    static uint32_t *primes      = NULL;  /* nth prime is primes[n-1] */
    static seq_t  *prefix_sum    = NULL;  /* prefix_sum[i] = sum of primes[0..i] */
    static size_t  prime_cnt     = 0;     /* number of primes stored */
    static size_t  prime_cap     = 0;     /* capacity of the allocated arrays */

    /* We need at least 2*n-1 primes. */
    size_t required = 2 * n - 1;

    /* Initialise the storage if this is the first call. */
    if (prime_cap == 0) {
        prime_cap = 16;
        primes = (uint32_t*)malloc(prime_cap * sizeof(*primes));
        prefix_sum = (seq_t*)malloc(prime_cap * sizeof(*prefix_sum));
        if (!primes || !prefix_sum) return 0;   /* out of memory (unlikely) */

        /* The first two primes are 2 and 3. */
        primes[0] = 2;
        prefix_sum[0] = (seq_t)2;
        prime_cnt = 1;

        if (prime_cnt < prime_cap) {
            primes[prime_cnt] = 3;
            prefix_sum[prime_cnt] = prefix_sum[prime_cnt - 1] + (seq_t)3;
            prime_cnt++;
        }
    }

    /* Grow storage as needed. */
    while (prime_cnt < required) {
        /* Resize arrays if capacity exceeded. */
        if (prime_cnt >= prime_cap) {
            prime_cap <<= 1; /* double the capacity */
            uint32_t *new_primes = (uint32_t*)realloc(primes, prime_cap * sizeof(*primes));
            seq_t    *new_prefix = (seq_t*)realloc(prefix_sum, prime_cap * sizeof(*prefix_sum));
            if (!new_primes || !new_prefix) return 0; /* out of memory */
            primes = new_primes;
            prefix_sum = new_prefix;
        }

        /* Find next prime after the last stored one. */
        uint32_t candidate = primes[prime_cnt - 1] + 2; /* start with an odd number */
        while (true) {
            /* Test divisibility by known primes up to sqrt(candidate). */
            seq_t sqrt_seq = floor_sqrt((seq_t)candidate);
            uint32_t sqrt_val = (uint32_t)sqrt_seq;
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt; i++) {
                uint32_t p = primes[i];
                if (p > sqrt_val) break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) break;           /* candidate is prime */
            candidate += 2;                 /* skip even numbers */
        }

        /* Store the newly found prime and update prefix sum. */
        primes[prime_cnt] = candidate;
        prefix_sum[prime_cnt] = prefix_sum[prime_cnt - 1] + (seq_t)candidate;
        prime_cnt++;
    }

    /* Sum of the first (2*n-1) primes is at prefix_sum[required-1]. */
    seq_t sum = prefix_sum[required - 1];
    /* The n-th prime is primes[n-1]. */
    seq_t mod_prime = (seq_t)primes[n - 1];
    return sum % mod_prime;
}
