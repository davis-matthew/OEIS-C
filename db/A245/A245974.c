/*
 * A245974  Tower of 7's mod n.
 * https://oeis.org/A245974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 282, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A245974 – Tower of 7's mod n
   Formula: a(n) = 7^a(φ(n)) (mod n)   for n > 10
            a(n) = (7^7) (mod n)      for 1 ≤ n ≤ 10 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

/* compute Euler's totient φ(m) for a positive uint64_t m */
static idx_t _phi_u64(idx_t m)
{
    idx_t result = m;
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            while (tmp % p == 0)
                tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1)
        result -= result / tmp;
    return result;
}

/* DEPRECATED: original comment suggests a(n)=n*(n+1)/2 – ignore for this sequence */

/* recursive evaluation of a(n) */
seq_t a(idx_t n)
{
    if (n <= 1)          /* defined: a(1)=0, a(0) unused */
        return (seq_t)0;

    if (n <= 10)         /* small n: direct 7^7 mod n */
        return mod_pow((seq_t)7, (uint64_t)7, (seq_t)n);

    /* general case: 7^(a(φ(n))) mod n */
    idx_t phi = _phi_u64(n);
    seq_t exp = a(phi);
    /* exp fits in idx_t because a(phi) < phi <= n */
    return mod_pow((seq_t)7, (uint64_t)exp, (seq_t)n);
}
