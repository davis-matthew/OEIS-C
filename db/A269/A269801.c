/*
 * A269801  Total sum of the divisors of the primes p,q such that n=p+q and
 *          p>=q.
 * https://oeis.org/A269801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2) * A061358(n)
 * Range:     u8 n <= 53, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A061358(n) = number of unordered prime pairs (p,q), p≥q, with p+q = n   */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Assumes seq_t and idx_t are defined elsewhere by the build system. */
seq_t a(idx_t n)
{
    /* Static state to cache primes up to the largest n seen. */
    static idx_t max_n = 0;
    static bool *is_prime = NULL;
    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_cnt = 0;

    /* Helper to build (or rebuild) the sieve up to limit. */
    if (n > max_n) {
        /* Allocate new is_prime array up to n. */
        bool *new_is = (bool *)realloc(is_prime, (size_t)(n + 1) * sizeof(bool));
        if (!new_is) return (seq_t)0;  /* fallback on allocation failure */
        is_prime = new_is;

        /* Mark all as prime initially; later we clear composites. */
        for (idx_t i = 0; i <= n; ++i)
            is_prime[i] = (i >= 2);

        /* Classic Eratosthenes sieve. */
        for (idx_t p = 2; p * p <= n; ++p) {
            if (!is_prime[p]) continue;
            for (idx_t j = p * p; j <= n; j += p)
                is_prime[j] = false;
        }

        /* Rebuild the prime list. */
        primes_cnt = 0;
        if (primes_cap == 0) {
            primes_cap = 16;
            primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            if (!primes) return (seq_t)0;
        }

        for (idx_t i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                    if (!tmp) return (seq_t)0;
                    primes = tmp;
                }
                primes[primes_cnt++] = (uint64_t)i;
            }
        }
        max_n = n;
    }

    /* Count unordered prime pairs summing to n. */
    size_t count = 0;
    if (n >= 4) {  /* smallest possible sum of two primes ≥ 2+2 = 4 */
        for (size_t idx = 0; idx < primes_cnt; ++idx) {
            uint64_t q = primes[idx];
            if (q > n / 2) break;         /* ensures p >= q */
            uint64_t p = n - q;
            if (p <= max_n && is_prime[p]) {
                ++count;                  /* pair (p,q) found */
            }
        }
    }

    /* Each pair contributes n+2 = sigma(p)+sigma(q). */
    seq_t result = (seq_t)(n + 2) * (seq_t)count;
    return result;
}
