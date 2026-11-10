/*
 * A378751  a(n) = A000203(n) - A048673(n).
 * https://oeis.org/A378751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) - (A003961(n)+1)/2, where A003961 replaces each
 *            prime p by the next prime
 * Range:     i8 n <= 23, i16 n <= 1535, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 9807 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    seq_t sigma = 1;
    seq_t shifted = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            seq_t np = (seq_t)next_prime(p);
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
                shifted *= np;
            }
            sigma *= term;
        }
    }
    if (m > 1) {
        sigma *= ((seq_t)m + 1);
        shifted *= (seq_t)next_prime(m);
    }
    return sigma - (shifted + 1) / 2;
}
