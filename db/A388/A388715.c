/*
 * A388715  Numbers with superdiagonal prime signature.
 * https://oeis.org/A388715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer with superdiagonal prime signature
 *            (the exponents of its prime factorization satisfy e_i ≥ i for
 *            the i‑th prime)
 * Range:     u8 n <= 89, u16 n <= 7196, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Return the n-th element of A388715.  n ≥ 1.  Caches previously
   computed terms so that arbitrary order of calls is efficient. */
seq_t a(idx_t n)
{
    /* Static storage for memoization */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cap        = 0;      /* allocated capacity */
    static size_t len        = 0;      /* number of terms currently stored */
    static uint64_t next_candidate = 2;/* next integer to evaluate (≥ 2) */

    /* Initialize on first call */
    if (len == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory */
        cache[0] = 1;                  /* a(1) = 1 */
        len = 1;
        next_candidate = 2;
    }

    /* Return cached value if already available */
    if (n <= len) {
        return cache[n - 1];
    }

    /* Generate terms until we have the n-th one */
    while (len < n) {
        /* Scan successive candidates */
        for (;;) {
            uint64_t cand = next_candidate;
            next_candidate = cand + 1;   /* advance for next iteration */

            /* Factor the candidate */
            uint64_t primes[32], exps[32];
            size_t cnt = 0;
            factor_u64(cand, primes, exps, &cnt);

            /* Check superdiagonal condition: exponent ≥ its index (1‑based) */
            bool ok = true;
            for (size_t i = 0; i < cnt; ++i) {
                if (exps[i] < (uint64_t)(i + 1)) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                /* Grow the cache if necessary */
                if (len >= cap) {
                    cap <<= 1;               /* double capacity */
                    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    if (!new_cache) return 0;/* out of memory */
                    cache = new_cache;
                }
                cache[len++] = (seq_t)cand;   /* store the new term */
                break;                       /* move to next term */
            }
        }
    }

    return cache[n - 1];
}
