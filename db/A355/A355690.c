/*
 * A355690  Dirichlet inverse of A152822, where A152822 is the characteristic
 *          function of numbers not congruent to 2 mod 4.
 * https://oeis.org/A355690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Dirichlet inverse of A152822 (characteristic function of
 *            numbers not ≡2 (mod 4)). Multiplicative: - Let n = 2^e · Π p_i,
 *            where the p_i are distinct odd primes. If any p_i appears with
 *            exponent ≥ 2 => a(n) = 0. - Otherwise a(n) = (-1)^k · a(2^e),
 *            where k is the number of odd primes. - Power...
 * Range:     i8 n <= 9981, i16 n <= 9981, i32 n <= 9981, i64 n <= 9981,
 *            i128 n <= 9981, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but return 0 */

    /* extract exponent of two */
    idx_t e = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        e++;
        tmp >>= 1;
    }
    uint64_t odd = (uint64_t)tmp;          /* remaining odd part */

    /* count distinct odd prime factors and check exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    if (odd > 1) {
        factor_u64(odd, primes, exps, &cnt);
        for (size_t i = 0; i < cnt; ++i)
            if (exps[i] > 1) return 0;    /* exponent ≥ 2 gives zero */
    } else {
        cnt = 0;                         /* no odd primes */
    }

    /* pre‑compute a(2^e) values up to 63 (enough for all 64‑bit n) */
    static seq_t pow2[64];
    static seq_t pref[64];
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        pow2[0] = 1;        pref[0] = 1;           /* a(1)    */
        if (64 > 1) {        /* a(2)    */
            pow2[1] = 0;
            pref[1] = 1;     /* 1 + 0   */
        }
        for (idx_t i = 2; i < 64; ++i) {
            pow2[i] = -pref[i - 2];               /* -(sum up to i-2) */
            pref[i] = pref[i - 1] + pow2[i];      /* extend prefix sum */
        }
    }

    seq_t val = pow2[e];
    if (cnt & 1)            /* multiply by (-1)^cnt */
        val = -val;
    return val;
}
