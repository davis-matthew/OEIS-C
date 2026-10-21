/*
 * A274536  a(n) = 6 * sigma(n).
 * https://oeis.org/A274536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * sigma(n)
 * Range:     u8 n <= 16, u16 n <= 2905, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma(n) = sum of all positive divisors of n */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *spf_cache = NULL;   /* smallest prime factor for each integer */
static seq_t    *sigma_cache = NULL; /* sigma values (sum of divisors) */
static size_t    spf_size = 0;        /* size of spf_cache array (in elements) */
static size_t    sigma_size = 0;      /* size of sigma_cache array (in elements) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* not used, but defined for safety */

    /* Extend caches if necessary */
    if ((size_t)n >= sigma_size) {
        size_t new_limit = (size_t)n;   /* we need indices 0..new_limit */

        /* Allocate new arrays */
        uint64_t *new_spf = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));
        seq_t    *new_sigma = (seq_t*)malloc((new_limit + 1) * sizeof(seq_t));

        /* Compute smallest prime factors up to new_limit */
        memset(new_spf, 0, (new_limit + 1) * sizeof(uint64_t));
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (new_spf[i] == 0) {
                new_spf[i] = i;
                if ((uint64_t)i * i <= (uint64_t)new_limit) {
                    for (uint64_t j = (uint64_t)i * i; j <= (uint64_t)new_limit; j += i)
                        if (new_spf[(size_t)j] == 0) new_spf[(size_t)j] = i;
                }
            }
        }

        /* Compute sigma values using the spf table */
        new_sigma[1] = 1;
        for (idx_t i = 2; i <= new_limit; ++i) {
            uint64_t p = new_spf[i];
            idx_t    m = i / (idx_t)p;

            /* Count exponent of p in i */
            idx_t cnt = 1;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }

            /* Compute (p^(cnt+1) - 1) / (p - 1) */
            seq_t power = 1;
            for (idx_t k = 0; k < cnt + 1; ++k)
                power *= (seq_t)p;
            seq_t numerator   = power - 1;
            seq_t denominator = (seq_t)p - 1;

            new_sigma[i] = new_sigma[m] * numerator / denominator;
        }

        /* Replace old caches */
        free(spf_cache);
        free(sigma_cache);
        spf_cache   = new_spf;
        sigma_cache = new_sigma;
        spf_size    = new_limit + 1;
        sigma_size  = new_limit + 1;
    }

    /* Return 6 * sigma(n) */
    return ((seq_t)6) * sigma_cache[(size_t)n];
}
