/*
 * A349431  Dirichlet convolution of A003602 (a Kimberling paraphrase) with
 *          A055615 (Dirichlet inverse of n).
 * https://oeis.org/A349431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A023900(n) if n is a power of 2, else A023900(n)/2, where
 *            A023900(n) = Product_{p|n} (1-p)
 * Range:     i8 n <= 222, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 9754 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t m = n;
    if ((m & 1) == 0) {
        result *= (seq_t)(-1);
        while ((m & 1) == 0) m >>= 1;
    }
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result *= ((seq_t)1 - (seq_t)p);
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result *= ((seq_t)1 - (seq_t)m);
    if ((n & (n - 1)) == 0) return result;
    return result / 2;
}
