/*
 * A255878  First differences of A256188.
 * https://oeis.org/A255878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A256188(n+1) - A256188(n), where A256188 replaces each
 *            triangular number k(k+1)/2 in the positive integers with 1..k
 * Range:     i8 n <= 239, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9845 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A256188_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t pos = 1;
    idx_t m = 1;
    idx_t k = 1;
    idx_t tri = 1;
    while (1) {
        if (m == tri) {
            if (n >= pos && n <= pos + k - 1) {
                return (seq_t)(n - pos + 1);
            }
            pos += k;
            k++;
            tri = k * (k + 1) / 2;
        } else {
            if (n == pos) return (seq_t)m;
            pos++;
        }
        m++;
    }
}

seq_t a(idx_t n)
{
    return A256188_a(n + 1) - A256188_a(n);
}
