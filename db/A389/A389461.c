/*
 * A389461  a(n) = sigma(n) - A075423(n) - n.
 * https://oeis.org/A389461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) - A075423(n) - n, where A075423(n) = rad(n) - 1
 * Range:     i8 n <= 95, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9631 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t t = n;
    seq_t sig = 1;
    seq_t rad = 1;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            seq_t term = 1, pw = 1;
            while (t % p == 0) {
                t /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            sig *= term;
            rad *= (seq_t)p;
        }
    }
    if (t > 1) {
        sig *= ((seq_t)t + 1);
        rad *= (seq_t)t;
    }
    return sig - (rad - 1) - (seq_t)n;
}
