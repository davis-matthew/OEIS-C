/*
 * A289509  Numbers k such that the gcd of the indices j for which the j-th
 *          prime prime(j) divides k is 1.
 * https://oeis.org/A289509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 9457, u32 n <= 9457, u64 n <= 9457,
 *            u128 n <= 9457, big any n
 * Helpers:   factor_u64, gcd, prime_pi, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A289509:  numbers k such that the gcd of the indices j of all primes
 *            dividing k is 1.
 *
 * The implementation generates terms by testing each integer k
 * sequentially.  Even numbers always satisfy the condition, whereas
 * odd numbers are verified by factorising k (using factor_u64) and
 * computing the gcd of the prime indices via prime_pi.  A cache is
 * maintained so that repeated calls to a(idx_t n) are fast.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

/* Return the n-th term of A289509 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety: index starts at 1 */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;
    static size_t   cache_cnt = 0;
    static uint64_t last_k   = 1;           /* last integer inspected */

    /* generate terms until we have at least n of them */
    while (cache_cnt < (size_t)n) {
        ++last_k;                    /* candidate k */

        bool good = false;

        /* every even number works */
        if ((last_k & 1ULL) == 0) {
            good = true;
        } else {
            /* odd number: factorise and test gcd of prime indices */
            const size_t MAX_FACT = 64;
            uint64_t p[MAX_FACT];
            uint64_t e[MAX_FACT];
            size_t  cnt = 0;

            factor_u64(last_k, p, e, &cnt);
            if (cnt > 0) {
                seq_t g = prime_pi(p[0]);   /* index of first prime factor */
                for (size_t i = 1; i < cnt; ++i) {
                    seq_t j = prime_pi(p[i]); /* index of next factor */
                    g = gcd(g, j);
                    if (g == 1) break;      /* early exit */
                }
                good = (g == 1);
            }
        }

        if (good) {
            /* grow cache if necessary */
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                if (!cache) abort();
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
