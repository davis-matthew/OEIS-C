/*
 * A296021  Number of primes of the form 4*k+1 <= 4*n+1.
 * https://oeis.org/A296021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes of the form 4*k+1 that are <= 4*n+1
 * Range:     u8 n <= 932, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static data to cache primes up to the largest limit seen so far   */
    static uint64_t *primes     = NULL;  /* array of found primes */
    static size_t prime_cnt     = 0;     /* number of primes stored  */
    static idx_t  last_limit   = 0;     /* largest limit s.t. primes are built   */

    /* compute the required limit */
    idx_t limit = 4 * n + 1;

    /* If we have not built primes up to this limit, rebuild the sieve   */
    if (limit > last_limit) {
        /* allocate and initialize the sieve array */
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime) {
            /* Out of memory – return 0 as a safe fallback                */
            return (seq_t)0;
        }
        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
        if (limit >= 0) is_prime[0] = false;
        if (limit >= 1) is_prime[1] = false;

        /* simple Eratosthenes sieve */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    is_prime[j] = false;
                }
            }
        }

        /* build the primes array */
        free(primes);                    /* free old storage if any     */
        prime_cnt = 0;
        /* worst case number of primes <= limit is limit itself,
           which is small for the intended use (≤ ~40001 for n≤10000) */
        primes = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
        if (!primes) {
            free(is_prime);
            return (seq_t)0;
        }
        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                primes[prime_cnt++] = (uint64_t)i;
            }
        }

        free(is_prime);
        last_limit = limit;
    }

    /* count primes ≡ 1 (mod 4) up to the limit */
    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > (uint64_t)limit) break;
        if ((p & 3ULL) == 1ULL) ++count;
    }

    return count;
}
