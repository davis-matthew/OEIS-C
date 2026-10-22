/*
 * A382331  If n = Product (p_j^k_j) then a(n) = -Sum ((-1)^k_j * p_j).
 * https://oeis.org/A382331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{p^e‖n} (-1)^(e+1) * p * (i.e. add p if the exponent e
 *            is odd, subtract p if e is even)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(1) = 0 */
    seq_t sum = 0;
    uint64_t m = (uint64_t)n;      /* work on an unsigned copy */

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        int exp = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            exp++;
        }
        sum += (exp & 1) ? (seq_t)2 : -(seq_t)2;
    }

    /* handle odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            sum += (exp & 1) ? (seq_t)p : -(seq_t)p;
        }
    }

    /* if any prime factor > sqrt(n) remains */
    if (m > 1) {
        sum += (seq_t)m;  /* exponent 1, odd */
    }

    return sum;
}
