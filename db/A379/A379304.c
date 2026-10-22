/*
 * A379304  Number of integer partitions of n with a unique prime part.
 * https://oeis.org/A379304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{prime p <= n} P_composite(n-p) * where
 *            P_composite(m) counts partitions of m using only composite parts
 *            (including 1)
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
seq_t a(idx_t n)
{
    static seq_t *pcomp = NULL;
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t pcomp_limit = 0;

    if (n < 2) {
        return 0;
    }

    if (pcomp_limit < n) {
        /* Allocate & initialize new arrays */
        size_t new_limit = (size_t)n;
        seq_t *new_pcomp = (seq_t *)calloc(new_limit + 1, sizeof(seq_t));
        if (!new_pcomp) return 0;
        new_pcomp[0] = 1;

        /* Sieve of Eratosthenes to find primes up to n */
        bool *isPrime = (bool *)malloc((new_limit + 1) * sizeof(bool));
        if (!isPrime) {
            free(new_pcomp);
            return 0;
        }
        for (size_t i = 0; i <= new_limit; ++i) isPrime[i] = true;
        isPrime[0] = isPrime[1] = false;
        for (idx_t i = 2; (idx_t)(i * i) <= n; ++i) {
            if (isPrime[i]) {
                for (idx_t j = i * i; j <= n; j += i) {
                    isPrime[j] = false;
                }
            }
        }

        /* Build prime list */
        size_t pc = 0;
        for (size_t i = 2; i <= new_limit; ++i) if (isPrime[i]) ++pc;
        idx_t *new_primes = (idx_t *)malloc(pc * sizeof(idx_t));
        if (!new_primes) {
            free(isPrime);
            free(new_pcomp);
            return 0;
        }
        for (size_t i = 2, k = 0; i <= new_limit; ++i) {
            if (isPrime[i]) new_primes[k++] = (idx_t)i;
        }
        prime_cnt = pc;
        free(primes);
        primes = new_primes;

        /* Compute partitions into composite parts (including 1) */
        for (idx_t c = 1; c <= n; ++c) {
            if (c == 1 || !isPrime[c]) {
                for (idx_t m = c; m <= n; ++m) {
                    new_pcomp[m] += new_pcomp[m - c];
                }
            }
        }

        free(isPrime);
        free(pcomp);
        pcomp = new_pcomp;
        pcomp_limit = n;
    }

    seq_t sum = 0;
    for (size_t k = 0; k < prime_cnt; ++k) {
        idx_t p = primes[k];
        if (p > n) break;
        sum += pcomp[n - p];
    }
    return sum;
}
