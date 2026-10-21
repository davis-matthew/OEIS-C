/*
 * A282016  Least k > 0 such that phi(n) divides phi(n+k).
 * https://oeis.org/A282016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 such that φ(n) divides φ(n+k)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the Euler totient φ for a given integer m.  The build supplies
   a fast implementation in utils/phi.h. */
#include "../utils/phi.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Pre‑compute totients up to LIMIT in an array cache. */
#define LIMIT 500000U
static seq_t *phi_cache = NULL;
static size_t phi_cache_len = 0;
static bool phi_cache_initialised = false;

static void init_phi_cache(void)
{
    if (phi_cache_initialised) return;

    phi_cache_len = LIMIT + 1;
    phi_cache = (seq_t *)calloc(phi_cache_len, sizeof(seq_t));
    if (phi_cache == NULL) exit(EXIT_FAILURE);

    /* Initialise φ(n) = n for all n. */
    for (size_t i = 0; i < phi_cache_len; ++i)
        phi_cache[i] = (seq_t)i;

    /* Euler sieve to compute the totient values. */
    for (size_t i = 2; i < phi_cache_len; ++i)
    {
        if (phi_cache[i] == (seq_t)i)          /* i is prime */
        {
            seq_t p = (seq_t)i;
            for (size_t j = i; j < phi_cache_len; j += i)
                phi_cache[j] -= phi_cache[j] / p;
        }
    }
    phi_cache_initialised = true;
}

seq_t a(idx_t n)
{
    /* Ensure the totient table is ready. */
    if (!phi_cache_initialised) init_phi_cache();

    /* φ(n) : use cache if possible, otherwise the generic φ() function. */
    seq_t phi_n = (n <= LIMIT) ? phi_cache[(size_t)n] : phi((idx_t)n);

    /* Search for the smallest k>0 such that φ(n+k) % φ(n) == 0. */
    for (seq_t k = 1; ; ++k)
    {
        idx_t m = n + (idx_t)k;            /* m = n + k */
        seq_t phi_m = (m <= LIMIT) ? phi_cache[(size_t)m] : phi((idx_t)m);
        if (phi_m % phi_n == 0) return k;
    }
}
