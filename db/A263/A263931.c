/*
 * A263931  a(n) = binomial(2*n, n) / Product(p prime | n < p <= 2*n).
 * https://oeis.org/A263931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) / Product_{p prime, n < p <= 2*n} p
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 46, u64 n <= 103,
 *            u128 n <= 197, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/build_primes_upto.h"
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 2*n will comfortably fit in idx_t for the intended range. */
    idx_t limit = (idx_t)(2 * n);

    /* All primes up to 2*n. */
    uint64_t *primes = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &primes, &cnt);

    seq_t res = (seq_t)1;

    /* For each prime, compute its exponent in (2n choose n) and
       subtract one if the prime lies in (n,2n].  Multiplying p^exp
       does not overflow because the final result fits in seq_t
       whenever it fits in the current build. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p > (uint64_t)limit) break;            /* safety */

        uint64_t exp = 0;
        uint64_t tmp = (uint64_t)limit;           /* 2*n */
        while (tmp) { tmp /= p; exp += tmp; }
        tmp = (uint64_t)n;
        while (tmp) { tmp /= p; exp -= 2 * tmp; }
        if (p > (uint64_t)n) exp -= 1;
        if (exp == 0) continue;

        for (uint64_t e = 0; e < exp; ++e) {
            res *= (seq_t)p;
        }
    }

    free(primes);
    return res;
}
