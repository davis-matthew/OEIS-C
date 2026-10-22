/*
 * A335278  First index of strictly decreasing prime quartets.
 * https://oeis.org/A335278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335278 – first index of strictly decreasing prime quartets.
   The function searches for indices i such that
   g(i) = prime(i+1)-prime(i) satisfies g(i) > g(i+1) > g(i+2),
   stores the 1‑based index i+1 of the first prime in each quartet,
   and returns the n‑th such index. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* n is 1‑based according to OEIS.  Return 0 for n==0. */
    if (n == 0) return (seq_t)0;

    /* Static state to avoid recomputation when a() is called
       multiple times with non‑monotonic n. */
    static idx_t *quartet_idx = NULL; /* 1‑based indices of quartets */
    static size_t quartet_len = 0;     /* number of already found quartets */
    static idx_t sieve_limit = 0;      /* current sieving limit */

    /* Keep expanding the sieve until we have at least n quartets. */
    while (quartet_len < n) {
        /* Determine new limit for the sieve. */
        idx_t new_limit;
        if (sieve_limit == 0) {
            /* Rough guess: 200 primes per 1st n indices. */
            new_limit = n * (idx_t)200;
            if (new_limit < 1000) new_limit = 1000;
        } else {
            new_limit = sieve_limit * 2;
        }

        /* Allocate composite flag array. */
        unsigned char *is_comp = (unsigned char *)calloc((size_t)new_limit + 1, 1);
        if (!is_comp) return (seq_t)0;          /* allocation failure */

        /* Simple sieve of Eratosthenes. */
        for (idx_t p = 2; (p * p) <= new_limit; ++p) {
            if (!is_comp[p]) {
                for (idx_t j = p * p; j <= new_limit; j += p)
                    is_comp[j] = 1;
            }
        }

        /* Count primes to allocate storage. */
        size_t primes_cnt = 0;
        for (idx_t i = 2; i <= new_limit; ++i)
            if (!is_comp[i]) ++primes_cnt;

        /* Store primes in an array of 64‑bit integers. */
        uint64_t *primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
        if (!primes) {
            free(is_comp);
            return (seq_t)0;
        }
        size_t pi = 0;
        for (idx_t i = 2; i <= new_limit; ++i)
            if (!is_comp[i]) primes[pi++] = (uint64_t)i;

        free(is_comp);  /* no longer needed */

        /* Find strictly decreasing quartets. */
        idx_t *new_quartets = NULL;
        size_t new_len   = 0;
        size_t new_cap   = 0;

        for (size_t i = 0; i + 3 < primes_cnt; ++i) {
            uint64_t g1 = primes[i + 1] - primes[i];
            uint64_t g2 = primes[i + 2] - primes[i + 1];
            uint64_t g3 = primes[i + 3] - primes[i + 2];
            if (g1 > g2 && g2 > g3) {
                if (new_len == new_cap) {
                    size_t ncap = new_cap ? new_cap * 2 : 1;
                    idx_t *tmp = (idx_t *)realloc(new_quartets, ncap * sizeof(idx_t));
                    if (!tmp) {
                        free(primes);
                        free(new_quartets);
                        return (seq_t)0;
                    }
                    new_quartets = tmp;
                    new_cap = ncap;
                }
                /* 1‑based index of the first prime in the quartet */
                new_quartets[new_len++] = (idx_t)(i + 1);
            }
        }

        free(primes);  /* primes no longer needed */

        /* Replace the old quartet list with the new extended one. */
        free(quartet_idx);
        quartet_idx   = new_quartets;
        quartet_len   = new_len;
        sieve_limit   = new_limit;
    }

    /* Return the requested term (0‑based conversion). */
    return (seq_t)quartet_idx[n - 1];
}
