/*
 * A303336  Number of rectangles with semiprime area and dimensions p,q where
 *          n = p+q and p <= q.
 * https://oeis.org/A303336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} [Omega(i*(n-i)) = 2]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses precomputation of the Omega function
   (number of prime factors counted with multiplicity) up to n.
   Hints: Omega(i*j) = Omega(i) + Omega(j), so we only need the Omega
   values for all numbers up to n to evaluate the sum. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for Omega values */
static uint8_t *omega_cache = NULL;      /* Omega[i] for 0 <= i <= limit */
static size_t   omega_cache_limit = 0;   /* maximum i for which cache is valid */

/* ensure that omega_cache is computed up to at least n */
static void ensure_omega(idx_t n)
{
    size_t needed = (size_t)n;
    if (needed <= omega_cache_limit)
        return;   /* already available */

    /* Allocate a new cache array */
    uint8_t *new_cache = (uint8_t*)calloc(needed + 1, sizeof(uint8_t));
    if (!new_cache) exit(1);   /* allocation failure */

    /* Smallest prime factor array for range 0..needed */
    uint32_t *spf = (uint32_t*)calloc(needed + 1, sizeof(uint32_t));
    if (!spf) { free(new_cache); exit(1); }

    /* Simple sieve to fill spf */
    size_t i, j;
    for (i = 2; i <= needed; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= needed) {
                for (j = i * i; j <= needed; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* Compute Ω(i) using spf */
    new_cache[0] = 0;
    new_cache[1] = 0;
    for (i = 2; i <= needed; ++i) {
        uint32_t p = spf[i];
        new_cache[i] = new_cache[i / p] + 1;
    }

    /* Replace old cache */
    free(omega_cache);
    free(spf);
    omega_cache = new_cache;
    omega_cache_limit = needed;
}

seq_t a(idx_t n)
{
    /* Ensure Ω values are available up to n */
    ensure_omega(n);

    seq_t count = 0;
    idx_t half = n / 2;   /* floor(n/2) */

    for (idx_t i = 1; i <= half; ++i) {
        idx_t j = n - i;                      /* j = n-i */
        /* Ω(i*j) = Ω(i) + Ω(j) */
        if ((size_t)(omega_cache[i] + omega_cache[j]) == 2) {
            ++count;
        }
    }
    return count;
}
