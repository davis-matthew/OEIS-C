/*
 * A330571  Square of number of unordered factorizations of n as n = i*j.
 * https://oeis.org/A330571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((d(n)+1)/2)², where d(n) is the number of divisors of n
 * Range:     u8 n <= 839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;          /* undefined for offset 1, but safe */
    }
    uint64_t m = (uint64_t)n;
    seq_t d = (seq_t)1;          /* divisor count starts at 1 */

    for (uint64_t p = 2; p * p <= m; ++p) {
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp > 0) {
            d *= (seq_t)(exp + 1);
        }
    }
    if (m > 1) {                  /* remaining prime factor */
        d *= (seq_t)2;
    }

    seq_t k = (d + (seq_t)1) / (seq_t)2;   /* ceil(d/2) */
    return k * k;
}
