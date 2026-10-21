/*
 * A277708  a(n) = Least prime divisor of n with an odd exponent, or 1 if n is
 *          a perfect square.
 * https://oeis.org/A277708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime divisor of n with an odd exponent, or 1 if n
 *            is a perfect square
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* n < 1 is undefined – return 0 for safety */
        return (seq_t)0;
    if (n == 1)          /* 1 is a perfect square */
        return (seq_t)1;

    uint64_t val = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(val, primes, exps, &cnt);

    uint64_t cand = 0;   /* 0 means no odd exponent found yet */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] & 1ULL) {
            if (cand == 0 || primes[i] < cand)
                cand = primes[i];
        }
    }

    return (cand == 0) ? (seq_t)1 : (seq_t)cand;
}
