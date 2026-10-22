/*
 * A358088  Number of pairs (s,t) with s and t squarefree, 1 <= s < t <= n and
 *          s | t.
 * https://oeis.org/A358088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=2..n, k squarefree} (2^ω(k) - 1), where ω(k) is the
 *            number of distinct prime factors of k. The recurrence relation
 *            is a(n)-a(n-1) = 2^ω(n)-1 if n is squarefree, otherwise 0
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset starts at 1 */
    if (n == 1) return 0;         /* a(1) = 0 */

    /* allocate arrays for Möbius μ and distinct‑prime count ω */
    idx_t size = n + 1;
    int *mu = (int*)malloc(size * sizeof(int));
    uint8_t *omega = (uint8_t*)malloc(size * sizeof(uint8_t));
    bool *composite = (bool*)malloc(size * sizeof(bool));
    if (!mu || !omega || !composite) {
        free(mu); free(omega); free(composite);
        return 0;                 /* allocation failure */
    }

    /* initialise */
    memset(composite, 0, size * sizeof(bool));
    memset(mu, 0, size * sizeof(int));
    memset(omega, 0, size * sizeof(uint8_t));
    mu[1] = 1;

    /* linear sieve: compute μ and ω up to n */
    size_t primes_cap = size / 10 + 1;
    uint32_t *primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    size_t prime_cnt = 0;
    if (!primes) { free(mu); free(omega); free(composite); return 0; }

    for (idx_t i = 2; i <= n; ++i) {
        if (!composite[i]) {
            if (prime_cnt == primes_cap) {           /* enlarge prime list */
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) { free(mu); free(omega); free(composite); return 0; }
            }
            primes[prime_cnt++] = (uint32_t)i;
            mu[i] = -1;
            omega[i] = 1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            unsigned __int128 prod = (unsigned __int128)i * (unsigned __int128)p;
            if (prod > n) break;
            idx_t k = (idx_t)prod;
            composite[k] = true;
            if (i % p == 0) {
                mu[k] = 0;
                omega[k] = omega[i];
                break;
            } else {
                mu[k] = -mu[i];
                omega[k] = omega[i] + 1;
            }
        }
    }

    /* accumulate the answer */
    seq_t ans = 0;
    for (idx_t k = 2; k <= n; ++k) {
        if (mu[k] != 0) {                        /* squarefree */
            seq_t pow2 = 1;
            for (uint8_t t = 0; t < omega[k]; ++t)
                pow2 <<= 1;                      /* 2^ω(k) */
            ans += pow2 - 1;
        }
    }

    free(mu); free(omega); free(composite); free(primes);
    return ans;
}
