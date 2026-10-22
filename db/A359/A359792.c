/*
 * A359792  a(n) = (-1)^A003415(n), where A003415 is the arithmetic derivative
 *          of n.
 * https://oeis.org/A359792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^A003415(n), where A003415(n) is the arithmetic
 *            derivative of n
 * Range:     i8 n <= 9081, i16 n <= 9081, i32 n <= 9081, i64 n <= 9081,
 *            i128 n <= 9081, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation strategy:  n = 2^k * m with m odd.  If k > 0 then A003415(n) is odd iff k = 1.
   If k = 0 then A003415(n) is odd iff the number of odd prime factors with odd
   multiplicity in m is odd.  This is computed by trial‑division. */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = n;
    /* Count factors of 2. */
    uint64_t k = 0;
    while ((m & 1ULL) == 0) {
        k++;
        m >>= 1;
    }
    if (k > 0) {
        /* Even case: derivative odd iff exponent of 2 is exactly 1. */
        return (k == 1) ? (seq_t)-1 : (seq_t)1;
    }
    /* Odd n: parity of the derivative equals parity of the sum of odd exponents. */
    int parity = 0;   /* 0 = even, 1 = odd */
    for (uint64_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            int eparity = 0;
            while (m % i == 0) {
                eparity ^= 1;   /* toggle for each factor occurrence */
                m /= i;
            }
            parity ^= eparity;
        }
    }
    if (m > 1) {          /* remaining prime factor with exponent 1 */
        parity ^= 1;
    }
    return (parity ? (seq_t)-1 : (seq_t)1);
}
