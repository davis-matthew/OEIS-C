/*
 * A262209  Inverse Möbius transform of A002654.
 * https://oeis.org/A262209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} A002654(d); multiplicative with a(p^e) = e+1 if
 *            p==2, floor(e/2)+1 if p ≡ 3 (mod 4), (e+1)*(e+2)/2 if p ≡ 1 (mod
 *            4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1, not used but safe */

    uint64_t m = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;
        if (p == 2) {
            term = (seq_t)e + (seq_t)1;
        } else if (p % 4ULL == 3ULL) {
            term = (seq_t)(e / 2) + (seq_t)1;
        } else {  /* p % 4 == 1 */
            term = ((seq_t)(e + 1) * (seq_t)(e + 2)) / (seq_t)2;
        }
        res *= term;
    }
    return res;
}
