/*
 * A292596  a(1) = a(2) = 0; for n > 2, a(n) = A010051(n) + 2*a(floor(n/2)).
 * https://oeis.org/A292596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0} [⌊n/2^k⌋ is an odd prime]·2^k For n≥1, with
 *            a(1)=a(2)=0. This direct implementation follows the OEIS
 *            definition by examining each term of the halving chain of n and
 *            setting the k‑th bit whenever an odd prime occurs. It uses the
 *            provided is_prime function from the utils library. The...
 * Range:     u8 n <= 767, u16 n <= 9485, u32 n <= 9485, u64 n <= 9485,
 *            u128 n <= 9485, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    seq_t result = 0;
    seq_t bit     = (seq_t)1;
    idx_t  m      = n;

    while (m > 1) {
        if ((m & (idx_t)1ULL) && is_prime(m))
            result |= bit;
        m >>= 1;
        bit <<= 1;
    }
    return result;
}
