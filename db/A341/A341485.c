/*
 * A341485  Number of ways to write n as an ordered sum of 8 nonprime numbers.
 * https://oeis.org/A341485
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 8-tuples of positive nonprime integers
 *            summing to n
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Minimum sum with 8 parts is 8. For n<8 the result is 0. */
    if (n < (idx_t)8)
        return (seq_t)0;

    /* Allocate sieve array for primes up to n. */
    bool *is_prime = (bool*)calloc((size_t)(n + 1), sizeof(bool));
    if (!is_prime)
        return (seq_t)0;   /* allocation failed, return 0 as safe fallback */

    /* Sieve of Eratosthenes */
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i <= n; ++i)
        is_prime[i] = true;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    /* Build list of nonprime numbers up to n (include 1). */
    idx_t *nonprime = NULL;
    size_t np_cap = 128;
    size_t np_len = 0;
    nonprime = (idx_t*)malloc(np_cap * sizeof(idx_t));
    if (!nonprime) {
        free(is_prime);
        return (seq_t)0;
    }
    for (idx_t i = 1; i <= n; ++i) {
        if (!is_prime[i]) {
            if (np_len >= np_cap) {
                np_cap *= 2;
                idx_t *tmp = (idx_t*)realloc(nonprime, np_cap * sizeof(idx_t));
                if (!tmp) { free(nonprime); free(is_prime); return (seq_t)0; }
                nonprime = tmp;
            }
            nonprime[np_len++] = i;
        }
    }

    /* DP arrays */
    seq_t *prev = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *next = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!prev || !next) {
        free(prev); free(next); free(nonprime); free(is_prime);
        return (seq_t)0;
    }
    prev[0] = (seq_t)1;

    for (int step = 0; step < 8; ++step) {
        /* zero next */
        for (idx_t i = 0; i <= n; ++i)
            next[i] = (seq_t)0;

        for (size_t k = 0; k < np_len; ++k) {
            idx_t p = nonprime[k];
            for (idx_t s = p; s <= n; ++s) {
                next[s] += prev[s - p];
            }
        }

        /* swap prev and next for next iteration */
        seq_t *tmp = prev;
        prev = next;
        next = tmp;
    }

    seq_t result = prev[n];

    /* Clean up */
    free(prev); free(next);
    free(nonprime);
    free(is_prime);

    return result;
}
