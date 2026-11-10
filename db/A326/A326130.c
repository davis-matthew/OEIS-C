/*
 * A326130  a(n) = gcd(A000120(n), A294898(n)) = gcd(A000120(n),
 *          sigma(n)-A005187(n)).
 * https://oeis.org/A326130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A000120(n), |sigma(n) - A005187(n)|), A005187(n) =
 *            Sum_{k>=0} floor(n/2^k)
 * Range:     u8 n <= 713, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9940 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A326130_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static seq_t A326130_sigma(idx_t n)
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

seq_t a(idx_t n)
{
    seq_t pop = 0;
    seq_t s5187 = 0;
    idx_t m = n;
    while (m) {
        pop += (seq_t)(m & 1);
        s5187 += (seq_t)m;
        m >>= 1;
    }
    seq_t sg = A326130_sigma(n);
    seq_t d = (sg >= s5187) ? (sg - s5187) : (s5187 - sg);
    return A326130_gcd(pop, d);
}
