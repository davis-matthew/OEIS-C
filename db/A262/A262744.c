/*
 * A262744  Remainder when sum of first n primes is divided by n-th triangular
 *          number.
 * https://oeis.org/A262744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = (Sum_{k=1..n} prime(k)) mod (n*(n+1)/2)
 * Range:     u8 n <= 13, u16 n <= 485, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; return 0 for n==0 if called by mistake. */
    if (n == 0) return 0;

    /* Static cache of primes and their prefix sums. */
    static uint64_t *primes   = NULL;   /* primes[0..current_n-1] */
    static seq_t   *prefix   = NULL;   /* prefix[0] = 0, prefix[i] = sum of first i primes */
    static idx_t   current_n = 0;      /* number of primes currently stored */
    static size_t  primes_cap = 0;
    static size_t  prefix_cap = 0;

    /* Grow primes array if needed. */
    if (n > current_n) {
        if (primes_cap < (size_t)n) {
            size_t new_cap = primes_cap ? primes_cap * 2 : 16;
            while (new_cap < (size_t)n) new_cap <<= 1;
            uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
            primes = tmp;
            primes_cap = new_cap;
        }

        /* Grow prefix array if needed. */
        if (prefix_cap < (size_t)(n + 1)) { /* +1 for prefix[0] */
            size_t new_cap = prefix_cap ? prefix_cap * 2 : 16;
            while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(prefix, new_cap * sizeof(seq_t));
            prefix = tmp;
            prefix_cap = new_cap;
        }

        /* Ensure prefix[0] is initialized. */
        if (prefix_cap > 0) prefix[0] = (seq_t)0;

        /* Generate missing primes. */
        uint64_t candidate = (current_n == 0) ? 2ULL : primes[current_n - 1] + 2ULL;
        if (current_n == 0) candidate = 2ULL;
        else if (primes[current_n - 1] == 2ULL) candidate = 3ULL;

        while (current_n < n) {
            if (is_prime(candidate)) {
                primes[current_n] = candidate;
                /* Update prefix sum. */
                prefix[current_n + 1] = prefix[current_n] + (seq_t)candidate;
                current_n++;
            }
            /* Advance to next candidate (skip even numbers). */
            if (candidate == 2ULL)
                candidate = 3ULL;
            else
                candidate += 2ULL;
        }
    }

    /* Compute triangular number and return the remainder. */
    seq_t triangular = (seq_t)n * ((seq_t)n + 1) / 2;
    return prefix[n] % triangular;
}
