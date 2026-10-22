/*
 * A341133  Number of ways to write n as an ordered sum of 4 prime powers
 *          (including 1).
 * https://oeis.org/A341133
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered quadruples of prime powers (including
 *            1) that sum to n
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External linkage required by the build */
seq_t a(idx_t n)
{
    /* 0 or 1 are meaningless for this sequence; return 0 for n<4. */
    if (n < 4)
        return 0;

    /* Static state to cache results up to the largest n requested. */
    static idx_t max_n = 0;
    static seq_t *dp1 = NULL, *dp2 = NULL, *dp3 = NULL, *dp4 = NULL;
    static idx_t *prime_power = NULL;
    static size_t pp_cnt = 0, pp_cap = 0;

    /* Rebuild all tables if a larger n is requested. */
    if (n > max_n) {
        /* Free existing arrays if any. */
        free(dp1);
        free(dp2);
        free(dp3);
        free(dp4);
        free(prime_power);

        max_n = n;

        /* Allocate DP arrays.  Use calloc to initialise to zero. */
        dp1 = (seq_t *)calloc((size_t)(max_n + 1), sizeof(seq_t));
        dp2 = (seq_t *)calloc((size_t)(max_n + 1), sizeof(seq_t));
        dp3 = (seq_t *)calloc((size_t)(max_n + 1), sizeof(seq_t));
        dp4 = (seq_t *)calloc((size_t)(max_n + 1), sizeof(seq_t));
        if (!dp1 || !dp2 || !dp3 || !dp4) {
            /* Out of memory – return 0.  The build environment will
               report a runtime failure if this occurs. */
            free(dp1); free(dp2); free(dp3); free(dp4);
            return 0;
        }

        /* Sieve of Eratosthenes for primes up to max_n. */
        bool *is_prime = (bool *)calloc((size_t)(max_n + 1), sizeof(bool));
        if (!is_prime) {
            free(dp1); free(dp2); free(dp3); free(dp4);
            return 0;
        }
        for (idx_t i = 2; i <= max_n; ++i)
            is_prime[i] = true;
        for (idx_t i = 2; i <= max_n / i; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= max_n; j += i)
                    is_prime[j] = false;
            }
        }

        /* Build list of prime powers (including 1).  1 is inserted first. */
        pp_cap = 64;
        prime_power = (idx_t *)malloc(pp_cap * sizeof(idx_t));
        if (!prime_power) {
            free(is_prime); free(dp1); free(dp2); free(dp3); free(dp4);
            return 0;
        }
        pp_cnt = 0;
        /* Insert 1 explicitly. */
        prime_power[pp_cnt++] = 1;

        for (idx_t p = 2; p <= max_n; ++p) {
            if (!is_prime[p])
                continue;
            /* Generate powers of p. */
            uint64_t val = p;
            while (val <= max_n) {
                if (pp_cnt == pp_cap) {
                    pp_cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(prime_power, pp_cap * sizeof(idx_t));
                    if (!tmp) {
                        free(is_prime);
                        free(dp1); free(dp2); free(dp3); free(dp4);
                        free(prime_power);
                        return 0;
                    }
                    prime_power = tmp;
                }
                prime_power[pp_cnt++] = (idx_t)val;
                if (val > max_n / p)
                    break;
                val *= p;
            }
        }
        free(is_prime);

        /* Mark dp1 entries: 1 if value is a prime power. */
        for (size_t k = 0; k < pp_cnt; ++k) {
            idx_t v = prime_power[k];
            dp1[v] = 1;
        }

        /* Convolution 1 -> 2 ---------------------------------------- */
        for (size_t k = 0; k < pp_cnt; ++k) {
            idx_t v = prime_power[k];
            for (idx_t i = v; i <= max_n; ++i) {
                dp2[i] += dp1[i - v];
            }
        }

        /* Convolution 2 -> 3 ---------------------------------------- */
        for (size_t k = 0; k < pp_cnt; ++k) {
            idx_t v = prime_power[k];
            for (idx_t i = v; i <= max_n; ++i) {
                dp3[i] += dp2[i - v];
            }
        }

        /* Convolution 3 -> 4 ---------------------------------------- */
        for (size_t k = 0; k < pp_cnt; ++k) {
            idx_t v = prime_power[k];
            for (idx_t i = v; i <= max_n; ++i) {
                dp4[i] += dp3[i - v];
            }
        }
    }

    /* Return the precomputed value. */
    return dp4[n];
}
