/*
 * A390691  The largest divisor of n that is a Loeschian number (A003136).
 * https://oeis.org/A390691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = largest divisor of n that is a Loeschian number.
 *            Multiplicative: a(p^e) = p^e if p ≡ 1 (mod 3) or p = 3, a(p^e) =
 *            p^(2⋅⌊e/2⌋) if p ≡ 2 (mod 3)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* offset is 1 – return 0 for n=0 if called */
    if (n == 1) return (seq_t)1;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t exp_for_a;

        if (p % 3ULL == 2ULL) {
            exp_for_a = (e / 2) * 2;          /* 2⋅⌊e/2⌋ */
        } else {
            exp_for_a = e;                    /* keep full exponent */
        }

        if (exp_for_a > 0) {
            res = res * big_pow((seq_t)p, (seq_t)exp_for_a);
        }
    }
    return res;
}
