/*
 * A264771  Primes of the form n^2 + phi(n).
 * https://oeis.org/A264771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that can be written as k^2 + φ(k), where k
 *            = 1, 2, 3, … and we take the primes in the natural order of k
 * Range:     u8 n <= 7, u16 n <= 37, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   calc_phi, ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/calc_phi.h"
#include "../utils/isPrimeGeneric.h"

static seq_t *prime_cache = NULL;     /* cached prime values   */
static size_t cache_cap   = 0;        /* allocated cache size  */
static size_t cache_len   = 0;        /* number of cached primes */
static idx_t   last_k     = 0;        /* last integer k processed */

seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (n <= cache_len)
        return prime_cache[n - 1];

    /* Generate new primes until we have n terms. */
    while (cache_len < n) {
        ++last_k;                                   /* next k */
        seq_t k_sq   = (seq_t)last_k * (seq_t)last_k;
        seq_t phi_k  = calc_phi((seq_t)last_k);
        seq_t cand   = k_sq + phi_k;

        if (isPrimeGeneric(cand)) {
            /* Grow cache if needed. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&prime_cache, sizeof(seq_t));
            prime_cache[cache_len++] = cand;
        }
    }
    return prime_cache[n - 1];
}
