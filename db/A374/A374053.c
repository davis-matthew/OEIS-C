/*
 * A374053  Multiplicative with a(3^k) = 0, a(p^(2k)) = 0 and a(p^(2k+1)) = 1
 *          if p == 1 (mod 3), and a(p^(2k)) = 1 and a(p^(2k+1)) = 0, if p ==
 *          -1 (mod 3).
 * https://oeis.org/A374053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e | n of * f(p,e) where *
 *            f(3,e) = 0 * f(p,e) (p≡1 mod3) = (e is odd) ? 1 : 0 * f(p,e)
 *            (p≡2 mod3) = (e is even) ? 1 : 0 * (otherwise) = 1 * (Thus a(n)
 *            is either 0 or 1.)
 * Range:     u8 n <= 9863, u16 n <= 9863, u32 n <= 9863, u64 n <= 9863,
 *            u128 n <= 9863, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return 1;
    }

    /* Allocate space for prime factors; 64 slots is more than enough for 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factor n into primes */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Any factor 3 kills the product */
        if (p == 3) {
            return 0;
        }

        uint64_t mod3 = p % 3U;
        if (mod3 == 1ULL) {            /* p ≡ 1 (mod 3) */
            if ((e & 1ULL) == 0ULL) {  /* even exponent → 0 */
                return 0;
            }
        } else {                        /* p ≡ 2 (mod 3) */
            if ((e & 1ULL) != 0ULL) {  /* odd exponent → 0 */
                return 0;
            }
        }
    }

    return 1;
}
