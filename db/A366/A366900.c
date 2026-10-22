/*
 * A366900  a(n) is the number of real roots of the derivative of the
 *          cyclotomic polynomial Phi(n, 1/x).
 * https://oeis.org/A366900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of real roots of the derivative of the
 *            cyclotomic polynomial Φ(n,1/x). Let r be the exponent of 2 in n
 *            and k the number of distinct odd prime factors of n/2^r. Then
 *            a(n) = 0 if n is a power of 2 or n = 1; a(n) = 2k‑1 if r = 0 or
 *            r = 1; a(n) = 2k if r ≥ 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0, and n == 0 is never requested */
    idx_t temp = n;
    int r = 0;
    /* count power of 2 in n */
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        r++;
    }
    /* temp is now the odd part of n */
    seq_t k = (seq_t)count_distinct_primes(temp);
    if (k == 0)                    /* n is a power of 2 */
        return 0;
    if (r <= 1)                    /* r == 0 (odd n) or r == 1 (2 * odd) */
        return 2 * k - 1;
    return 2 * k;                  /* r >= 2 and odd part present */
}
