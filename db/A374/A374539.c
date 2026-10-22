/*
 * A374539  The sum of the squares of the infinitary divisors of n.
 * https://oeis.org/A374539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(p^e) = Π_{k>=0, e_k=1}
 *            (p^(2^(k+1)) + 1), where e = Σ_k e_k·2^k is the binary expansion
 *            of the exponent e of the prime p
 * Range:     u8 n <= 14, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

/* contribution of a prime power p^e to the product a(n) */
static seq_t _inf_contrib(seq_t p, unsigned int e)
{
    seq_t prod = 1;
    unsigned int bit = 0;
    unsigned int rem = e;

    while (rem) {
        if (rem & 1) {
            /* exponent 2^(bit+1) fits in 64-bit for e <= 64 */
            seq_t term = pow_uint((uint64_t)p, (idx_t)(1ULL << (bit + 1))) + 1;
            prod *= term;
        }
        rem >>= 1;
        ++bit;
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but gracefully handle 0 */

    uint64_t t = (uint64_t)n;
    seq_t ans = 1;

    /* factor 2 separately */
    unsigned int e2 = 0;
    while ((t & 1ULL) == 0ULL && t > 0) {
        ++e2;
        t >>= 1;
    }
    if (e2)
        ans *= _inf_contrib((seq_t)2, e2);

    /* odd prime factors by trial division */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned int e = 0;
            while (t % p == 0) {
                ++e;
                t /= p;
            }
            ans *= _inf_contrib((seq_t)p, e);
        }
    }

    /* remaining prime factor, if any */
    if (t > 1)
        ans *= _inf_contrib((seq_t)t, 1);

    return ans;
}
