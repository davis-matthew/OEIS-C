/*
 * A347625  Number of partitions of n into at most 6 prime parts (counting 1
 *          as a prime).
 * https://oeis.org/A347625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347625:
   Number of partitions of n into at most 6 prime parts,
   treating 1 as a prime.  The function uses a dynamic
   programming sieve over the primes up to n. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Work with a size_t for array indices. */
    size_t N = (size_t)n;

    /* dp[t][s] will be stored in a one‑dimensional array:
       index = t * stride + s   (t = 0..6, s = 0..N). */
    const size_t stride = N + 1;
    const size_t total  = 7 * stride;

#if defined(__cplusplus)
    seq_t *dp = new seq_t[total];    /* default‑constructed for C++ */
#else
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
#endif

    if (dp == NULL) { return 0; }    /* allocation failure */

    dp[0] = (seq_t)1;                 /* dp[0][0] = 1 */

    /* Build the list of allowed part sizes: 1 and all primes ≤ N. */
    /* Allocate enough space for N+1 potential primes. */
#if defined(__cplusplus)
    uint32_t *primes = new uint32_t[N + 1];
#else
    uint32_t *primes = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
#endif
    if (primes == NULL) {
#if defined(__cplusplus)
        delete[] dp;
#else
        free(dp);
#endif
        return 0;
    }
    size_t prime_cnt = 0;

    if (N >= 1) {
        primes[prime_cnt++] = 1;          /* treat 1 as a prime */
    }

    if (N >= 2) {
        bool *is_prime = (bool *)calloc(N + 1, sizeof(bool));
        if (is_prime == NULL) {
#if defined(__cplusplus)
            delete[] dp;
            delete[] primes;
#else
            free(dp);
            free(primes);
#endif
            return 0;
        }

        /* Sieve of Eratosthenes. */
        for (size_t i = 2; i <= N; ++i) {
            is_prime[i] = true;
        }
        for (size_t i = 2; i * i <= N; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= N; j += i) {
                    is_prime[j] = false;
                }
            }
        }

        for (size_t i = 2; i <= N; ++i) {
            if (is_prime[i]) {
                primes[prime_cnt++] = (uint32_t)i;
            }
        }

        free(is_prime);
    }

    /* Dynamic programming over the primes. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        size_t p = (size_t)primes[pi];
        for (size_t t = 1; t <= 6; ++t) {
            size_t t_off  = t * stride;
            size_t t1_off = (t - 1) * stride;
            for (size_t s = p; s <= N; ++s) {
                dp[t_off + s] += dp[t1_off + (s - p)];
            }
        }
    }

    /* Sum partitions with at most 6 parts. */
    seq_t res = 0;
    for (size_t t = 0; t <= 6; ++t) {
        res += dp[t * stride + N];
    }

    /* Free allocated memory. */
#if defined(__cplusplus)
    delete[] dp;
    delete[] primes;
#else
    free(dp);
    free(primes);
#endif

    return res;
}
