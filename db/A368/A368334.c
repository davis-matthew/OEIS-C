/*
 * A368334  The number of terms of A054744 that are unitary divisors of n.
 * https://oeis.org/A368334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} [ e < p ? 1 : 2 ] (Multiplicative with a(p^e) =
 *            1 if e < p, 2 if e ≥ p)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"  /* provides factor_u64 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 return 0 safely. */
    if (n == 0)
        return (seq_t)0;

    /* factorize n */
    uint64_t pn = (uint64_t)n;
    uint64_t prime[64];
    uint64_t exp[64];
    size_t cnt = 0;
    factor_u64(pn, prime, exp, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exp[i] >= prime[i])
            result = result * (seq_t)2;
    }
    return result;
}
