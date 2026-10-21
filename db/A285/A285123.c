/*
 * A285123  Min(|d(k+1-i) - d(i)|, for i = 1..k), where d(1),..,d(k) are the
 *          divisors of prime(n) - 1.
 * https://oeis.org/A285123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(|d(k+1-i) - d(i)| for i = 1..k), * where d(1..k) are
 *            the divisors of prime(n) - 1. * Equivalent: a(n) = min_{d |
 *            (p-1)} | d - (p-1)/d |, p = prime(n)
 * Range:     u8 n <= 55, u16 n <= 8726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* p is the n-th prime */
    uint64_t m = (uint64_t)p - 1ULL;      /* m = prime(n) - 1 */

    /* Compute floor(sqrt(m)) without floating point */
    uint64_t sqrtN = 0;
    for (sqrtN = 1; (sqrtN + 1) * (sqrtN + 1) <= m; ++sqrtN) { }

    seq_t minDiff = (seq_t)m;           /* diff never exceeds m-1 */

    for (uint64_t d = 1; d <= sqrtN; ++d) {
        if (m % d == 0) {
            uint64_t c = m / d;
            uint64_t diff = (d > c) ? d - c : c - d;
            if (diff < (uint64_t)minDiff) {
                minDiff = (seq_t)diff;
                if (minDiff == (seq_t)0) break;   /* cannot be smaller */
            }
        }
    }
    return minDiff;
}
