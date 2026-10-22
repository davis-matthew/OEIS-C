/*
 * A322362  a(n) = gcd(n, A166590(n)), where A166590 is completely
 *          multiplicative with a(p) = p+2 for prime p.
 * https://oeis.org/A322362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A166590(n))
 * Range:     u8 n <= 35, u16 n <= 5631, u32 n <= 9913, u64 n <= 9913,
 *            u128 n <= 9913, big any n
 * Helpers:   _smallest_prime_factor, gcd, pow_seq_unsigned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A166590 is completely multiplicative with a(p)=p+2 for prime p */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/pow_seq_unsigned.h"

/* Compute A166590(n) for 1 <= n <= 2^64 */
static seq_t A166590(uint64_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    uint64_t m = n;

    while (m > 1)
    {
        uint64_t p = _smallest_prime_factor(m);
        uint64_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }
        seq_t base = (seq_t)(p + 2);
        seq_t pow_val = pow_seq_unsigned(base, e);
        prod *= pow_val;
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1 – behaviour for n==0 is unused */
        return (seq_t)0;

    seq_t m = (seq_t)n;
    seq_t val = A166590((uint64_t)n);
    return gcd(m, val);
}
