/*
 * A349326  a(n) is the number of prime powers (not including 1) that are
 *          bi-unitary divisors of n.
 * https://oeis.org/A349326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum over prime powers p^e||n of f(p^e), where f(p^e) = e
 *            if e is odd, and e-1 if e is even
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return 0;        /* a(1) = 0 */

    uint64_t x = (uint64_t)n;

    /* arrays big enough for all prime factors of a 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64(x, primes, exps, &cnt);

    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        if ((e & 1) == 0)
            e -= 1;      /* subtract 1 if exponent is even */
        res += e;
    }
    return res;
}
