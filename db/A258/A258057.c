/*
 * A258057  First differences of the arithmetic derivative sequence A003415.
 * https://oeis.org/A258057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A003415(n+1) - A003415(n) (first differences of the
 *            arithmetic derivative)
 * Range:     i8 n <= 62, i16 n <= 6142, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8452 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute the arithmetic derivative of a non‑negative integer n
   using the formula d(n) = ∑_{p|n} e_p * (n / p)  */
static seq_t derivative(idx_t n) {
    if (n <= 1) return 0;
    seq_t sum = 0;
    idx_t original = n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            sum += (seq_t)e * ((seq_t)original / (seq_t)p);
        }
    }
    if (m > 1) {            /* remaining prime factor */
        sum += (seq_t)1 * ((seq_t)original / (seq_t)m);
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t d_n    = derivative(n);
    seq_t d_np1  = derivative(n + 1);
    return d_np1 - d_n;
}
