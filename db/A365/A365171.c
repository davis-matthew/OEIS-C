/*
 * A365171  The number of divisors d of n such that gcd(d, n/d) is a square.
 * https://oeis.org/A365171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of
 *            floor((e+3)/4) + floor((e+4)/4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used (offset starts at 1) */
    uint64_t nn = (uint64_t)n;

    /* buffer large enough for all prime factors of a 64‑bit integer */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)exps[i];
        seq_t local = (seq_t)((e + 3) / 4) + (seq_t)((e + 4) / 4);
        res *= local;
    }
    return res;
}
