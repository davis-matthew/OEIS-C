/*
 * A286875  If n = Product (p_j^k_j) then a(n) = Sum (k_j >= 2, p_j^k_j).
 * https://oeis.org/A286875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p^k || n, k>=2} p^k (sum of unitary proper prime
 *            power divisors)
 * Range:     u8 n <= 255, u16 n <= 9889, u32 n <= 9889, u64 n <= 9889,
 *            u128 n <= 9889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;
    uint64_t m = n;
    seq_t sum = 0;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t val = 1;
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                val *= p;
            }
            if (exp >= 2) {
                sum += (seq_t)val;
            }
        }
    }
    /* Any remaining factor m > 1 is a prime with exponent 1, so nothing to add. */
    return sum;
}
