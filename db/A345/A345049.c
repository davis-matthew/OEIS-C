/*
 * A345049  a(n) = A173557(n) * A345001(n).
 * https://oeis.org/A345049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A173557(n) * A345001(n), with A173557(n) = Product_{p|n}
 *            (p-1) and A345001(n) = sigma(n) + n' - 2n (n' = arithmetic
 *            derivative)
 * Range:     i8 n <= 12, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t sig = 1;      /* sigma(n) */
    seq_t der = 0;      /* arithmetic derivative n' */
    seq_t prodm = 1;    /* product of (p-1) over distinct p | n */
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                ++e;
                pw *= (seq_t)p;
                term += pw;
            }
            sig *= term;
            der += (N / (seq_t)p) * (seq_t)e;
            prodm *= (seq_t)(p - 1);
        }
    }
    if (m > 1) {
        sig *= (seq_t)(m + 1);
        der += N / (seq_t)m;
        prodm *= (seq_t)(m - 1);
    }
    return prodm * (sig + der - (seq_t)2 * N);
}
