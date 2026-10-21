/*
 * A273976  Primes of the form: Sum_{j=0..k} (k-j)^j.
 * https://oeis.org/A273976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A273976: a(n) is the n‑th prime of the form
 *   S(k) = ∑_{j=0..k} (k−j)^j ,  k ≥ 2. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions supplied by the build system */
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Cache of primes already found and the next k to test. */
    static seq_t *primes = NULL;      /* array of found primes      */
    static size_t primes_len = 0;     /* number of primes stored    */
    static size_t primes_cap = 0;     /* allocated capacity         */
    static idx_t next_k = 1;          /* next k to test (will become 2) */

    /* Generate primes until we have at least n of them. */
    while (primes_len < n) {
        ++next_k;  /* k = 2, 3, 4, … */

        /* Compute S(k) = ∑_{i=0..k} i^{k-i}.  The i = 0 term is 0. */
        seq_t sum = 0;
        for (idx_t i = 1; i <= next_k; ++i) {
            idx_t exp = next_k - i;          /* non‑negative exponent */
            seq_t pow = big_pow((seq_t)i, (seq_t)exp);
            sum += pow;
        }

        /* If the sum is prime, store it in the cache. */
        if (is_prime_big(sum)) {
            if (primes_len >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = sum;
        }
    }

    /* Return the requested term (1‑based index). */
    return primes[n - 1];
}
