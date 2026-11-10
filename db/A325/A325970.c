/*
 * A325970  a(n) is the largest k <= A066503(n) such that k and (2n-sigma(n))
 *          [= A033879(n)] are relatively prime.
 * https://oeis.org/A325970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = largest k <= n - rad(n) with gcd(k, 2n - sigma(n)) = 1,
 *            or -1 if none; found by searching k downward
 * Range:     i8 n <= 143, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 97 terms through n = 9288 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A325970_gcd(seq_t x, seq_t y)
{
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = n;
    seq_t rad = 1;
    seq_t sig = 1;
    for (idx_t p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pk = 1;
            rad *= (seq_t)p;
            while (m % p == 0) {
                m /= p;
                pk *= (seq_t)p;
                term += pk;
            }
            sig *= term;
        }
    }
    if (m > 1) {
        rad *= (seq_t)m;
        sig *= ((seq_t)m + 1);
    }
    seq_t d = (seq_t)2 * (seq_t)n - sig;
    seq_t bound = (seq_t)n - rad;
    for (seq_t k = bound; k >= 0; k--) {
        if (A325970_gcd(k, d) == 1) return k;
    }
    return -1;
}
