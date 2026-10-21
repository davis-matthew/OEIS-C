/*
 * A268388  "Fermi-Dirac composites": numbers k for which A064547(k) > 1.
 * https://oeis.org/A268388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268388: numbers k for which the sum of the binary
   digits of the exponents in the prime factorisation of k is > 1
   (i.e. A064547(k) > 1).  The function searches for such numbers
   and caches the results so that subsequent calls are fast. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t  cache[20000];      /* storage for the first 20000 terms */
static size_t cache_len  = 0;    /* number of terms stored so far */
static uint64_t next_candidate = 2;   /* next integer to test */

/* return the nth term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined; safe return */

    /* extend the cache until it contains the n‑th term */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;

        /* factor cand into primes and exponents */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(cand, primes, exps, &cnt);

        /* sum the popcounts of the exponents */
        unsigned sumBits = 0;
        for (size_t i = 0; i < cnt; ++i) {
            sumBits += __builtin_popcountll(exps[i]);
            if (sumBits > 1) break;   /* early exit */
        }

        if (sumBits > 1) {             /* candidate satisfies the condition */
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
