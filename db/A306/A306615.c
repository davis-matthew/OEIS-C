/*
 * A306615  Least positive k such that 2n - p is prime where p is some prime
 *          divisor of n^k - 1, or 0 if no such k exists.
 * https://oeis.org/A306615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that 2n‑p is prime * where p is a
 *            prime divisor of n^k − 1, or 0 if no such k exists
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
/* utilities from the build environment */
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/powmod64.h"

/* least k such that 2n-p is prime with p | n^k-1, else 0 */
seq_t a(idx_t n)
{
    /* convert index to a native 64‑bit value for arithmetic */
    uint64_t NN = (uint64_t)n;
    uint64_t maxp = 2 * NN;

    /* populate primes up to 2n */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(maxp, &primes, &primes_cnt);

    uint64_t best = 0;   /* 0 represents “no suitable k found yet” */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t q = primes[i];           /* prime candidate for 2n - p */
        if (q >= maxp) break;

        uint64_t p = maxp - q;            /* potential prime divisor of n^k-1 */
        if (p < 2) continue;
        if (!is_prime(p)) continue;
        if (NN % p == 0) continue;       /* p cannot divide n^k-1 when p|n */

        /* multiplicative order of n modulo p */
        uint64_t order = p - 1;

        /* factor p-1 into primes */
        uint64_t fpr[64], fex[64];
        size_t fcnt = 0;
        factor_u64(p - 1, fpr, fex, &fcnt);

        for (size_t j = 0; j < fcnt; ++j) {
            uint64_t fac = fpr[j];
            while (order % fac == 0 &&
                   powmod64(NN % p, order / fac, p) == 1) {
                order /= fac;
            }
        }

        if (best == 0 || order < best) best = order;
    }

    free(primes);
    return (seq_t)best;
}
