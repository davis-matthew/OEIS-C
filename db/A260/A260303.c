/*
 * A260303  For the Collatz (3x+1) iteration starting at k, the sequence lists
 *          the smallest k such that prime(n) belongs to the trajectory of k,
 *          or 0 if no such k exists.
 * https://oeis.org/A260303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 967, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the smallest starting integer k such that the
   n‑th prime appears in the Collatz (3x+1) trajectory starting from k
   (the starting value k itself is not counted). If no such k exists
   within the search it returns 0.  The algorithm builds a sieve of
   primes up to the n‑th prime, then iterates over k incrementally,
   recording the first k that reaches each prime.
   The result is memoised so that repeated calls are O(1).   */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

static idx_t computed_max_n = 0;     /* largest n already processed   */
static seq_t *answer_cache = NULL;   /* answer_cache[0] unused;   answer_cache[n] = a(n) */

/* Build a simple boolean sieve of Eratosthenes up to 'limit'.
   is_prime[0] and is_prime[1] are set to 0.  The caller must free the returned
   pointer.  The array is of unsigned char, where 1 means prime. */
static void build_sieve(uint32_t limit, unsigned char **out)
{
    unsigned char *is_prime = (unsigned char*)calloc(limit + 1, sizeof(unsigned char));
    if (!is_prime) abort();
    /* initially mark all numbers >=2 as potentially prime */
    for (uint32_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
    *out = is_prime;
}

/* Recompute the cache up to 'n_max' if necessary.  This recomputes from
   scratch (the old cache is freed).  It is only called when a larger
   index is requested. */
static void recompute(idx_t n_max)
{
    if (n_max == 0) return;

    /* Determine the maximal prime we need to consider */
    uint32_t max_prime = get_prime((idx_t)n_max);

    /* Allocate and build a boolean sieve for primes up to max_prime */
    unsigned char *is_prime = NULL;
    build_sieve(max_prime, &is_prime);

    /* prime_k[p] holds the first k that reaches prime p; 0 means unknown yet */
    seq_t *prime_k = (seq_t*)calloc(max_prime + 1, sizeof(seq_t));
    if (!prime_k) { free(is_prime); abort(); }

    /* Search for primes incrementally */
    idx_t found = 0;
    for (idx_t k = 1; found < n_max; ++k) {
        seq_t cur = k;
        while (true) {
            if (cur == (seq_t)1) break;          /* reached the end of trajectory */
            if ((cur & (seq_t)1) == (seq_t)0)    /* even */
                cur >>= 1;
            else                                /* odd */
                cur = cur * (seq_t)3 + (seq_t)1;

            /* If the current value lies within the sieve range, check it */
            if (cur <= (seq_t)max_prime) {
                uint32_t v = (uint32_t)cur;
                if (is_prime[v] && prime_k[v] == (seq_t)0) {
                    prime_k[v] = k;
                    ++found;
                    if (found == n_max) break;
                }
            }
        }
        if (found == n_max) break;
    }

    /* Allocate the cache for results */
    seq_t *new_cache = (seq_t*)calloc(n_max + 1, sizeof(seq_t));
    if (!new_cache) { free(is_prime); free(prime_k); abort(); }

    /* Fill cache: for each prime index, store the recorded k */
    for (idx_t i = 1; i <= n_max; ++i) {
        uint32_t p = get_prime((idx_t)i);
        new_cache[i] = prime_k[p];
    }

    /* Replace old cache and clean up */
    if (answer_cache) free(answer_cache);
    answer_cache = new_cache;
    computed_max_n = n_max;

    free(prime_k);
    free(is_prime);
}

/* Public function: returns a(n) as defined in OEIS A260303 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset does not matter below 1 */
    if (n > computed_max_n) recompute(n);
    return answer_cache[n];
}
