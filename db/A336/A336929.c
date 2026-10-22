/*
 * A336929  a(n) = A331410(sigma(n)), where A331410 is totally additive with
 *          a(2) = 0 and a(p) = 1 + a(p+1) for odd primes.
 * https://oeis.org/A336929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A331410( sigma(n) ) where A331410 is totally additive
 *            with a(2) = 0 and a(p) = 1 + a(p + 1) for odd primes
 * Range:     u8 n <= 781, u16 n <= 9499, u32 n <= 9499, u64 n <= 9499,
 *            u128 n <= 9499, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"

/* recursive evaluation of A331410 on a 64‑bit integer */
static seq_t A331410_rec(uint64_t m)
{
    if (m <= 1) return (seq_t)0;
    if (m == 2) return (seq_t)0;          /* a(2) = 0 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);

    seq_t total = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        seq_t   e   = (seq_t)exps[i];
        if (p == 2) continue;            /* a(2) = 0, contributes nothing */

        seq_t ap = (seq_t)1 + A331410_rec(p + 1);  /* a(p) = 1 + a(p+1) for odd primes */
        total += ap * e;
    }
    return total;
}

/* public sequence function A336929 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out‑of‑range guard – unused in OEIS offset */

    seq_t sigma = sigma_of((idx_t)n);      /* sigma(n) = sum of divisors of n */
    uint64_t sigma64 = (uint64_t)sigma;    /* for n up to about 10000 sigma fits in 64 bits */

    return A331410_rec(sigma64);
}
