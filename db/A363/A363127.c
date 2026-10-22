/*
 * A363127  Number of non-modes in the multiset of prime factors of n.
 * https://oeis.org/A363127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non-modes in the multiset of prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    idx_t exps[64];       /* exponents of distinct prime factors   */
    size_t sz = 0;        /* number of distinct primes found       */

    idx_t x = n;
    for (idx_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            idx_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            exps[sz++] = e;
        }
    }
    if (x > 1) {          /* remaining prime factor > sqrt(n)    */
        exps[sz++] = 1;
    }
    if (sz == 0) return 0;    /* n == 1 case                           */

    idx_t maxe = 0;
    for (size_t i = 0; i < sz; ++i)
        if (exps[i] > maxe) maxe = exps[i];

    seq_t count = 0;
    for (size_t i = 0; i < sz; ++i)
        if (exps[i] < maxe) ++count;

    return count;
}
