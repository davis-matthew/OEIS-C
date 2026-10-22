/*
 * A342106  Numbers that are not arithmetic and not balanced.
 * https://oeis.org/A342106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer m such that neither τ(m) (τ is
 *            the divisor count) nor φ(m) divides σ(m) (σ is the sum of
 *            divisors). Computed by a simple incremental search and cached
 *            results for repeated queries
 * Range:     u8 n <= 34, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   divisor_count_u64, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

/* External‐linkage function required by the build system */
seq_t a(idx_t n)
{
    /* Return 0 for out‑of‑range index (offset is 1); this case is
       not used in the tests. */
    if (n < 1)
        return 0;

    /* Static cache of already computed terms. */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;

    /* Candidate integer to test next.  Starts from 1 and is advanced
       only when a new term is needed. */
    static uint64_t next_candidate = 1;

    /* Extend the cache until it contains at least n terms. */
    while (cache_sz < n) {
        /* Find next candidate that satisfies the condition. */
        do {
            ++next_candidate;

            uint64_t tau  = divisor_count_u64(next_candidate);
            seq_t    sigma = sigma_of(next_candidate);
            seq_t    phi_val = phi(next_candidate);   /* phi is a static helper */

            /* Check divisibility conditions */
            if ((sigma % (seq_t)tau) != 0 && (sigma % phi_val) != 0)
                break;
        } while (1);

        /* Grow the cache if necessary. */
        if (cache_sz >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache,
                                          cache_cap * sizeof(seq_t));
            if (!tmp) {          /* Allocation failure: return 0 as a safe
                                    fallback (should not occur under
                                    normal limits). */
                return 0;
            }
            cache = tmp;
        }

        /* Store the found term and update the size. */
        cache[cache_sz++] = (seq_t)next_candidate;
    }

    /* Return the requested term (0‑based index in the cache). */
    return cache[n - 1];
}
