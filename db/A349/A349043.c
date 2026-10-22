/*
 * A349043  Number of solutions to n = s + t such that 1 <= s <= t and s |
 *          2*t.
 * https://oeis.org/A349043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9564, u16 n <= 9564, u32 n <= 9564, u64 n <= 9564,
 *            u128 n <= 9564, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of divisors d of 2*n that are <= n/2
   Equivalently a(n) = tau(2*n) - 2 - (3|n)   (for n>=1) */
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* extract exponent of 2 from n */
    unsigned int exp2 = 0;
    uint64_t nn = (uint64_t)n;
    while ((nn & 1ULL) == 0ULL) {
        exp2++;
        nn >>= 1;
    }

    /* factor the odd part of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    if (nn > 1)
        factor_u64(nn, primes, exps, &cnt);

    /* divisor count of n */
    seq_t tau_n = 1;
    tau_n *= (seq_t)(exp2 + 1);
    for (size_t i = 0; i < cnt; ++i)
        tau_n *= (seq_t)(exps[i] + 1);

    /* divisor count of 2*n (increase exponent of 2 by 1) */
    seq_t tau_2n = tau_n * (seq_t)(exp2 + 2);
    tau_2n /= (seq_t)(exp2 + 1);

    /* subtract the divisors 1, 2, and possibly 3 */
    seq_t small = 2;                      /* 1 and 2 always divide 2*n */
    if ((n % 3) == 0) small += 1;         /* 3 divides 2*n iff 3|n */

    return tau_2n - small;
}
