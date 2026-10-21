/*
 * A294092  Numbers k == 119 (mod 120) such that 2^((k-1)/2), 3^((k-1)/2) and
 *          5^((k-1)/2) are congruent to 1 (mod k).
 * https://oeis.org/A294092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, grow_seq, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294092: Numbers k ≡ 119 (mod 120) for which
   2^((k‑1)/2), 3^((k‑1)/2) and 5^((k‑1)/2) ≡ 1 (mod k).
   Computation is performed by successive search of candidates
   of the form 120·m+119, testing the three modular
   exponentiations with the utility mod_pow.  Results are
   cached so that arbitrary order of calls is supported. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/append_candidate.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Cache of found terms.  For C++ the cast of realloc is required. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* m is the counter used to generate candidates: k = 120*m + 119. */
    static idx_t m = 1;

    if (n == 0)
        return 0; /* undefined for n < 1 */

    /* Grow cache until we have at least n items. */
    while (cache_len < (size_t)n) {
        /* Ensure capacity for one more element. */
        grow_seq(&cache, &cache_cap, cache_len + 1);

        /* Candidate k. */
        seq_t k = (seq_t)120 * (seq_t)m + (seq_t)119;
        ++m;

        /* Exponent e = (k-1)/2; fit into 64‑bit for the required range. */
        uint64_t e = (uint64_t)((k - (seq_t)1) / (seq_t)2);

        /* Test the congruences. */
        if (mod_pow((seq_t)2, e, k) == (seq_t)1 &&
            mod_pow((seq_t)3, e, k) == (seq_t)1 &&
            mod_pow((seq_t)5, e, k) == (seq_t)1) {

            /* Store the new term in the cache. */
            cache[cache_len++] = k;
        }
    }

    return cache[n-1];
}
