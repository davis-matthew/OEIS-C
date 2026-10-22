/*
 * A372516  Number of ones minus number of zeros in the binary expansion of
 *          the n-th prime number.
 * https://oeis.org/A372516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A000120(A000040(n)) - A080791(A000040(n))
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* static cache of primes */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t limit = 15;   /* current upper bound for sieve */

    /* generate primes if we do not yet have the n‑th one */
    if (primes_cnt < (size_t)n) {
        /* improve the initial bound if needed */
        if (limit < n * 15) limit = n * 15;

        while (1) {
            /* free yesterday's data if any */
            if (primes) {
                free(primes);
                primes = NULL;
                primes_cnt = 0;
            }

            /* sieve up to `limit` */
            size_t sz = (size_t)limit + 1;
            bool *is_prime = (bool*)malloc(sz);
            if (!is_prime) return (seq_t)0;   /* allocation failure */

            for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
            if (sz > 0) is_prime[0] = false;
            if (sz > 1) is_prime[1] = false;

            for (size_t i = 2; (size_t)i * i <= limit; ++i) {
                if (is_prime[i]) {
                    for (size_t j = i * i; j <= (size_t)limit; j += i)
                        is_prime[j] = false;
                }
            }

            /* count the primes to know how many to allocate */
            size_t cnt = 0;
            for (size_t i = 2; i <= (size_t)limit; ++i)
                if (is_prime[i]) ++cnt;

            if (cnt < (size_t)n) {
                /* not enough primes – double the limit and retry */
                free(is_prime);
                limit *= 2;
                continue;
            }

            /* store the primes */
            primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
            if (!primes) {
                free(is_prime);
                return (seq_t)0;
            }
            size_t idx = 0;
            for (size_t i = 2; i <= (size_t)limit; ++i)
                if (is_prime[i]) primes[idx++] = (uint64_t)i;

            primes_cnt = cnt;
            free(is_prime);
            break;   /* success */
        }
    }

    /* fetch the n‑th prime (1‑based index) */
    uint64_t p = primes[n - 1];

    /* count the number of 1 bits */
    unsigned int ones = __builtin_popcountll(p);

    /* bit length of the prime */
    unsigned int bits = 0;
    uint64_t temp = p;
    while (temp) { bits++; temp >>= 1; }

    /* number of 0 bits */
    unsigned int zeros = bits - ones;

    /* result = ones – zeros */
    int64_t diff = (int64_t)ones - (int64_t)zeros;
    return (seq_t)diff;
}
