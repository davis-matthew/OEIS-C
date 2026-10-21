/*
 * A280544  Expansion of 1/(1 - Sum_{k>=2} (1 - floor(2/d(k)))*x^k), where
 *          d(k) is the number of divisors (A000005).
 * https://oeis.org/A280544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into composite parts (OEIS
 *            A280544). a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 * Range:     u8 n <= 29, u16 n <= 50, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the n-th term of the sequence.  The function is memoised
   and works for arbitrary order of calls. */
seq_t a(idx_t n)
{
    /* Static state to store computed values and helper tables */
    static seq_t *dp = NULL;          /* dp[i] = number of compositions for i */
    static bool *is_composite = NULL; /* is_composite[i] == true iff i is composite */
    static size_t allocated = 0;      /* allocated size of arrays (dp, is_composite) */

    /* Ensure the tables are large enough for n */
    if (n >= allocated) {
        size_t new_len = (size_t)n + 1;
        dp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
        is_composite = (bool *)realloc(is_composite, new_len * sizeof(bool));

        /* Sieve of Eratosthenes for primes up to n */
        bool *is_prime = (bool *)calloc(new_len, sizeof(bool));
        if (is_prime) {
            /* 0 and 1 are not prime */
            for (size_t i = 2; i < new_len; ++i) {
                is_prime[i] = true;
            }
            for (size_t p = 2; p * p < new_len; ++p) {
                if (is_prime[p]) {
                    for (size_t mult = p * p; mult < new_len; mult += p) {
                        is_prime[mult] = false;
                    }
                }
            }
        }

        /* Fill the composite flag array */
        for (size_t i = 0; i < new_len; ++i) {
            is_composite[i] = (i >= 4 && !is_prime[i]);
        }
        free(is_prime);

        /* Initialise base case */
        dp[0] = 1;

        /* Extend the dynamic programming table */
        size_t start = allocated ? allocated : 1; /* start from 1 if we already had dp[0] */
        for (size_t i = start; i < new_len; ++i) {
            dp[i] = 0;
            for (size_t c = 4; c <= i; ++c) {
                if (is_composite[c]) {
                    dp[i] += dp[i - c];
                }
            }
        }
        allocated = new_len;
    }

    return dp[(size_t)n];
}
