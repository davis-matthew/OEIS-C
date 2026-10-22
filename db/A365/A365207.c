/*
 * A365207  The number of divisors d of n such that gcd(d, n/d) is a power of
 *          2 (A000079).
 * https://oeis.org/A365207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A365207.
 *  Number of divisors d of n such that gcd(d,n/d) is a power of 2.
 *  The function evaluates a(n) for n >= 1 as:
 *
 *      a(n) = (v₂(n)+1) * 2^{κ(n)},
 *
 *  where v₂(n) is the exponent of 2 in n and κ(n) is the number of
 *  distinct odd prime factors of n.  The function factors n by trial
 *  division up to √n; no additional table or memoisation is required.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)    /* offset starts at 1, but guard anyway */
        return (seq_t)0;

    idx_t temp = n;
    /* count exponent of 2 */
    idx_t e2 = 0;
    while ((temp & 1u) == 0u) {
        temp >>= 1;
        e2++;
    }

    /* start result with (e2+1) */
    seq_t result = (seq_t)(e2 + 1);

    /* count distinct odd prime factors */
    idx_t cntOdd = 0;
    idx_t m = temp;
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            cntOdd++;
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    if (m > 1)          /* leftover prime factor > sqrt(original n) */
        cntOdd++;

    /* multiply by 2^cntOdd */
    for (idx_t i = 0; i < cntOdd; ++i) {
        result <<= 1;
    }

    return result;
}
