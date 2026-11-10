/*
 * A300237  a(n) = n - A286594(n) = n - A285721(n, sigma(n)).
 * https://oeis.org/A300237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A285721(n, sigma(n)), where A285721(n,k) is the
 *            number of subtraction steps of the Euclidean algorithm on (n,k)
 *            until the two values are equal
 * Range:     u8 n <= 263, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 114 terms through n = 9346 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A000203_a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    return result;
}

static seq_t A285721_euclid_steps(seq_t x, seq_t y)
{
    seq_t steps = 0;
    while (x != y) {
        if (x > y) {
            seq_t q = x / y;
            seq_t rem = x % y;
            if (rem == 0) {
                steps += q - 1;
                break;
            }
            steps += q;
            x = rem;
        } else {
            seq_t q = y / x;
            seq_t rem = y % x;
            if (rem == 0) {
                steps += q - 1;
                break;
            }
            steps += q;
            y = rem;
        }
    }
    return steps;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t s = A000203_a(n);
    seq_t st = A285721_euclid_steps((seq_t)n, s);
    return (seq_t)n - st;
}
