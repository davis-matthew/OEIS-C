/*
 * A309478  Sum of the squarefree parts of the partitions of n into 3 parts.
 * https://oeis.org/A309478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} Sum_{i=j..floor((n-j)/2)} (i *
 *            mu(i)^2 + j * mu(j)^2 + (n-i-j) * mu(n-i-j)^2)
 * Range:     u8 n <= 16, u16 n <= 108, u32 n <= 4392, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here mu(m) is the Möbius function.  The implementation precomputes
   the Möbius values up to the current n via a sieve of smallest prime
   factors, so that each partition is evaluated efficiently. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for Möbius values (using smallest prime factors) */
    static idx_t *spf = NULL;      /* smallest prime factor array   */
    static int8_t *mu_cache = NULL;/* Möbius values (-1,0,1)       */
    static idx_t mu_size = 0;      /* number of entries in the cache */

    /* Expand cache if necessary */
    if (n + 1 > mu_size) {
        idx_t new_size = n + 1;
        free(spf);
        free(mu_cache);
        spf = (idx_t *)malloc(new_size * sizeof(idx_t));
        mu_cache = (int8_t *)malloc(new_size * sizeof(int8_t));
        if (!spf || !mu_cache) return 0;   /* fallback on allocation failure */

        /* initialise spf array */
        for (idx_t i = 0; i < new_size; ++i) spf[i] = 0;
        for (idx_t i = 2; i < new_size; ++i) {
            if (spf[i] == 0) {
                for (idx_t j = i; j < new_size; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }

        /* compute Möbius values using spf */
        mu_cache[0] = 0;
        mu_cache[1] = 1;
        for (idx_t i = 2; i < new_size; ++i) {
            idx_t p = spf[i];
            idx_t m = i / p;
            if (m % p == 0)
                mu_cache[i] = 0;
            else
                mu_cache[i] = -mu_cache[m];
        }
        mu_size = new_size;
    }

    /* Evaluate the double sum */
    seq_t sum = 0;
    idx_t max_j = n / 3;
    for (idx_t j = 1; j <= max_j; ++j) {
        idx_t max_i = (n - j) / 2;
        for (idx_t i = j; i <= max_i; ++i) {
            idx_t k = n - i - j;      /* k >= i >= j >= 1 in all valid traversals */
            if (mu_cache[i] != 0) sum += (seq_t)i;
            if (mu_cache[j] != 0) sum += (seq_t)j;
            if (mu_cache[k] != 0) sum += (seq_t)k;
        }
    }
    return sum;
}
