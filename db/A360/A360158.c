/*
 * A360158  a(n) is the number of unitary divisors of n that are odd squares
 *          minus the number of unitary divisors of n that are even squares.
 * https://oeis.org/A360158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} f(p^e) where * f(2^e) = 1 if e is odd, * 0 if e
 *            is even; * f(p^e) = 1 if e is odd (p odd), * 2 if e is even (p
 *            odd)
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
    /* Work with 64‑bit integer for factorization */
    uint64_t num = (uint64_t)n;

    seq_t result = 1;
    /* Temporary arrays: 64 is enough for any 64‑bit number */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(num, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if ((e & 1) == 0) {                /* even exponent */
            seq_t mul = (p == 2U) ? 0 : 2;
            /* Multiplying by 0 yields 0 for all further factors */
            result *= mul;
            if (result == 0)
                break;
        }
        /* if exponent is odd, local contribution is 1 → nothing changes */
    }

    return result;
}
