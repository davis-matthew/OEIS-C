/*
 * A367926  a(n) is the number of 2 X 2 matrices over the integers mod n that
 *          are invertible mod n for every permutation of their elements.
 * https://oeis.org/A367926
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} p^(4(e-1)) * (p^4 - 3p^3 + 9p^2 - 17p + 10)
 *            for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;              /* valid for offset 2 and above */

    /* arrays for prime factors and exponents (max 20 distinct primes for 64‑bit n) */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt;

    /* factorize n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* compute polynomial part: p^4 - 3p^3 + 9p^2 - 17p + 10 */
        seq_t p2 = p * p;
        seq_t p3 = p2 * p;
        seq_t p4 = p3 * p;
        seq_t poly = p4
                    - (seq_t)3 * p3
                    + (seq_t)9 * p2
                    - (seq_t)17 * p
                    + (seq_t)10;

        /* compute p^(4(e-1)) via big_pow (handles exp = 0) */
        seq_t pow_factor = big_pow(p, (seq_t)(4 * (e - 1)));

        /* accumulate the multiplicative contribution */
        result = result * pow_factor * poly;
    }

    return result;
}
