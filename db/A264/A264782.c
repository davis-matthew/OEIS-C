/*
 * A264782  a(n) = Sum_{d|n} möbius(d)^(n/d).
 * https://oeis.org/A264782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} μ(d)^(n/d) Non-recursive implementation: • For
 *            odd n > 1, a(n) = 0 (and a(1) = 1). • Let n = 2^m * u with u
 *            odd. * If n == 2, a(n) = 0. * If m == 1, a(n) = 2^r where r =
 *            number of distinct odd prime factors of u. * If m >= 2, a(n) =
 *            2^(r+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
seq_t a(idx_t n) {
    if (n == 1) return 1;
    /* Count powers of two */
    idx_t m = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        ++m;
        temp >>= 1;
    }
    if (n == 2) return 0;          /* Special case: a(2)=0 */
    if (m == 0) return 0;          /* Odd n > 1 */
 
    /* Count distinct odd prime factors of the odd part (temp) */
    idx_t r = 0;
    idx_t u = temp;                /* u is odd now */
    for (idx_t p = 3; p * p <= u; p += 2) {
        if (u % p == 0) {
            ++r;
            while (u % p == 0) u /= p;
        }
    }
    if (u > 1) ++r;                /* Remaining prime factor */
 
    idx_t shift = (m == 1) ? r : (r + 1);
    seq_t result = 1;
    for (idx_t i = 0; i < shift; ++i) result <<= 1;  /* result *= 2 each iteration */
    return result;
}
