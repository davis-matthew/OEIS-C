/*
 * A295663  a(n) = A295664(n) - A056169(n); 2-adic valuation of tau(n) minus
 *          the number of unitary prime divisors of n.
 * https://oeis.org/A295663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A295664(n) - A056169(n) = sum_{p^e||n} (v2(e+1) -
 *            (e==1?1:0))
 * Range:     u8 n <= 8155, u16 n <= 8155, u32 n <= 8155, u64 n <= 8155,
 *            u128 n <= 8155, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    if (n <= 1U) return 0;

    uint64_t temp = (uint64_t)n;
    seq_t sum = 0;

    /* factor 2 */
    unsigned e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e >= 2) {
        uint64_t t = e + 1;
        while ((t & 1ULL) == 0) {
            ++sum;
            t >>= 1;
        }
    }

    /* odd prime factors */
    for (uint64_t p = 3ULL; p <= temp / p; p += 2ULL) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e >= 2) {
                uint64_t t = e + 1;
                while ((t & 1ULL) == 0) {
                    ++sum;
                    t >>= 1;
                }
            }
        }
    }

    /* leftover prime factor (exponent 1) contributes 0 */

    return sum;
}
