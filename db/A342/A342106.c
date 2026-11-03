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
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   mem, num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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

            uint64_t tau  = num_divisors(next_candidate);
            seq_t    sigma = sigma_of(next_candidate);
            seq_t    phi_val = totient(next_candidate);   /* phi is a static helper */

            /* Check divisibility conditions */
            if ((sigma % (seq_t)tau) != 0 && (sigma % phi_val) != 0)
                break;
        } while (1);

        /* Grow the cache if necessary. */
        if (cache_sz >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
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
