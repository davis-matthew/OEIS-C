/*
 * A244739  Numbers k such that (prime(k) mod 5) == 0 (mod 3).
 * https://oeis.org/A244739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A244739: Numbers k such that the k-th prime p_k satisfies
 * (p_k mod 5) == 0 (mod 3), i.e. p_k ≡ 0 or 3 (mod 5).
 * Return the nth such index (1‑based).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Helper: test if a uint64_t is prime using stored primes */
static bool is_prime_candidate(uint64_t n, const uint64_t *primes, size_t primes_cnt) {
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate the nth term of the sequence */
seq_t a(idx_t n) {
    /* Static state: list of qualifying indices and list of primes used for tests */
    static seq_t *qual_indices = NULL;
    static size_t qual_cnt = 0, qual_cap = 0;

    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;

    /* Current state while generating primes */
    static uint64_t last_prime = 2;  /* current prime being examined */
    static idx_t last_k = 1;         /* its index k */

    /* Ensure prime list contains at least the prime 2 initially */
    if (prime_cnt == 0) {
        prime_cap = 8;
        prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
        prime_list[prime_cnt++] = 2;
    }

    /* Generate indices until we have at least n of them */
    while (qual_cnt < (size_t)n) {
        /* Check if current prime qualifies */
        if (last_prime % 5 == 0 || last_prime % 5 == 3) {
            if (qual_cnt == qual_cap) {
                qual_cap = qual_cap ? qual_cap * 2 : 8;
                qual_indices = (seq_t*)realloc(qual_indices,
                                              qual_cap * sizeof(seq_t));
            }
            qual_indices[qual_cnt++] = (seq_t)last_k;
        }

        /* Find next prime after 'last_prime' */
        uint64_t candidate = last_prime + 1;
        if (candidate <= 2) candidate = 3;          /* after 2 we start at 3 */
        if (candidate % 2 == 0) candidate++;        /* skip even numbers */

        while (!is_prime_candidate(candidate, prime_list, prime_cnt)) {
            candidate += 2;                         /* test only odd candidates */
        }

        /* Update state */
        last_prime = candidate;
        ++last_k;

        /* Store the new prime in the list for future tests */
        if (prime_cnt == prime_cap) {
            prime_cap = prime_cap ? prime_cap * 2 : 8;
            prime_list = (uint64_t*)realloc(prime_list,
                                            prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_cnt++] = last_prime;
    }

    return qual_indices[n - 1];
}
