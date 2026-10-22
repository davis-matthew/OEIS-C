/*
 * A336615  Numbers of the form p * m^2, where p is prime and m > 0 is not
 *          divisible by p.
 * https://oeis.org/A336615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A336615 – numbers of the form p · m²,
   where p is prime and m > 0 is not divisible by p. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* static state for memoisation ----------------------------------- */
    static seq_t *cache      = NULL;   /* array of computed terms   */
    static size_t cache_len  = 0;      /* number of terms stored   */
    static size_t cache_cap  = 0;      /* current capacity         */
    static uint64_t next_candidate = 2;/* next integer to test     */

    /* ensure we have at least n terms -------------------------------- */
    if ((size_t)n > cache_len) {
        /* enlarge capacity if necessary */
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n;
            while (new_cap < (size_t)n) new_cap <<= 1;
            if (cache == NULL) {
                cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
                if (!cache) abort();
            } else {
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache_cap = new_cap;
        }

        /* generate successive candidates until we have n terms */
        while (cache_len < (size_t)n) {
            uint64_t cand = next_candidate++;
            /* factorise cand (cand fits in 64‑bit for n≈10000) */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t   cnt;
            factor_u64(cand, primes, exps, &cnt);

            /* check the condition: exactly one exponent must be 1,
               all other exponents must be even */
            size_t oddcnt = 0;
            bool ok = true;
            for (size_t i = 0; i < cnt; ++i) {
                if (exps[i] & 1ULL) {          /* odd exponent       */
                    oddcnt++;
                    if (exps[i] != 1ULL) {     /* must be exactly 1 */
                        ok = false;
                        break;
                    }
                }
            }
            if (ok && oddcnt == 1) {
                cache[cache_len++] = (seq_t)cand;   /* store term   */
            }
        }
    }

    /* return requested term (1‑based index) */
    return cache[(size_t)n - 1];
}
