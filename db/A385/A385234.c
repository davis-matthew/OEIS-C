/*
 * A385234  a(n) is the number of partitions of n into primes of the form 4*k
 *          + 1.
 * https://oeis.org/A385234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into primes of the form 4*k + 1
 *            Computation: dynamic‐programming DP on the set of primes ≡ 1
 *            (mod 4). The routine is compatible with both C (seq_t an
 *            unsigned integral type) and C++ (seq_t a big‑integer class with
 *            a default constructor and operator+=). It allocate...
 * Range:     u8 n <= 167, u16 n <= 405, u32 n <= 1186, u64 n <= 4021,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* size of DP and sieve arrays */
    size_t sz = (size_t)n + 1;

#ifdef __cplusplus
    /* C++: use new to construct objects */
    seq_t *dp = new seq_t[sz];
    bool   *prime = new bool[sz];
#else
    /* C: use calloc to zero‑initialize primitives/integers */
    seq_t *dp = (seq_t*)calloc(sz, sizeof(seq_t));
    bool   *prime = (bool*)calloc(sz, sizeof(bool));
#endif

    if (!dp || !prime) {   /* allocation failure */
#ifndef __cplusplus
        if (dp) free(dp);
        if (prime) free(prime);
#endif
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;              /* empty partition */
    /* Initialise sieve table */
    for (size_t i = 2; i < sz; ++i)
        prime[i] = true;
    prime[0] = prime[1] = false;

    /* Sieve of Eratosthenes */
    for (size_t i = 2; (idx_t)i * (idx_t)i <= n; ++i) {
        if (prime[i]) {
            for (size_t j = i * i; j < sz; j += i)
                prime[j] = false;
        }
    }

    /* DP over primes of the form 4*k + 1 */
    for (size_t p = 2; p < sz; ++p) {
        if (!prime[p] || (p % 4) != 1)
            continue;
        for (size_t s = p; s < sz; ++s) {
            dp[s] += dp[s - p];
        }
    }

    seq_t result = dp[(size_t)n];

#ifndef __cplusplus
    free(dp);
    free(prime);
#else
    delete[] dp;
    delete[] prime;
#endif

    return result;
}
