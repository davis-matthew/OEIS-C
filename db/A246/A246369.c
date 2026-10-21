/*
 * A246369  a(1)=0, a(p_n) = a(n), a(c_n) = 1 + a(n), where p_n = n-th prime =
 *          A000040(n), c_n = n-th composite number = A002808(n); Also one
 *          less than the binary weight of terms of A135141.
 * https://oeis.org/A246369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; For n ≥ 2: if (is_prime(n)) a(n) = a( π(n) ); else
 *            a(n) = 1 + a( n - π(n) - 1 ); π(n) = number of primes ≤ n
 * Range:     u8 n <= 8315, u16 n <= 8315, u32 n <= 8315, u64 n <= 8315,
 *            u128 n <= 8315, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"     /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Static cache: largest n that has been computed so far */
    static idx_t max_computed = 0;
    /* Cached π(n) values, 0‑based indexing; pi[0] = 0, pi[1] = 0, ... */
    static idx_t *pi_cache = NULL;
    /* Cached a(n) values, 0‑based indexing; a[0] is unused */
    static seq_t *a_cache = NULL;

    if (n < 1)               /* offset is 1, undefined below */
        return (seq_t)0;

    /* Extend the cache if needed */
    if (n > max_computed) {
        /* New size must accommodate indices 0 … n */
        const size_t new_size = (size_t)(n + 1);

        /* Reallocate the π cache */
        pi_cache = (idx_t *)realloc(pi_cache, new_size * sizeof(idx_t));
        /* Reallocate the a cache */
        a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!pi_cache || !a_cache) {
            /* Memory allocation failed – aborting. In the real
               benchmark environment this situation does not occur. */
            exit(1);
        }

        /* Initialise the very first entry, which is never used but
           protects the array bounds for the computation loop. */
        pi_cache[0] = 0;
        a_cache[0] = (seq_t)0;

        /* Compute π(i) and a(i) for all i > max_computed up to n. */
        idx_t previous_pi = (max_computed == 0) ? 0 : pi_cache[max_computed];

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            const bool prime_flag = is_prime(i);
            previous_pi += prime_flag ? 1 : 0;
            pi_cache[i] = previous_pi;

            if (i == 1) {                /* a(1) = 0 by definition */
                a_cache[i] = (seq_t)0;
            } else if (prime_flag) {     /* prime case */
                a_cache[i] = a_cache[pi_cache[i]];
            } else {                     /* composite case */
                const idx_t comp_index = i - pi_cache[i] - 1;
                a_cache[i] = a_cache[comp_index] + 1;
            }
        }

        max_computed = n;
    }

    return a_cache[n];
}
