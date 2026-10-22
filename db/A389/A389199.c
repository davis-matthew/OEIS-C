/*
 * A389199  Numbers whose sum of squarefree divisors (A048250) is congruent to
 *          2 modulo 4.
 * https://oeis.org/A389199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 7191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A389199: numbers k whose odd part is a prime power p^e with
 * p ≡ 1 (mod 4).  The function scans integers in increasing order,
 * tests the condition, stores found terms in a cache, and returns
 * the nth term.  It uses a brute‑force check but stops as soon as
 * the required term has been found. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t len = 0;      /* number of terms stored */
    static size_t cap = 0;     /* current cache capacity */
    static idx_t last_checked = 0; /* last integer examined */

    /* Ensure we have at least n terms cached */
    while (len < n) {
        /* next candidate integer */
        idx_t cand = ++last_checked;

        /* compute odd part */
        idx_t odd = cand;
        while ((odd & 1) == 0)
            odd >>= 1;

        if (odd > 1) {
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;

            factor_u64(odd, primes, exps, &cnt);

            /* accept iff odd part is a single prime power and that prime ≡ 1 mod 4 */
            if (cnt == 1 && (primes[0] & 3) == 1) {
                /* grow cache if necessary */
                if (len == cap) {
                    size_t new_cap = cap ? cap * 2 : 16;
                    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                    if (!new_cache) abort(); /* allocation failure */
                    memcpy(new_cache, cache, len * sizeof(seq_t));
                    free(cache);
                    cache = new_cache;
                    cap = new_cap;
                }
                cache[len++] = (seq_t)cand;
            }
        }
    }

    return cache[n - 1];
}
