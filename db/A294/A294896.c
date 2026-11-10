/*
 * A294896  a(n) = gcd(A000203(n), A005187(n)).
 * https://oeis.org/A294896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A000203(n), A005187(n)), A000203 = sigma(n),
 *            A005187(n) = Sum_{k>=0} floor(n/2^k)
 * Range:     u8 n <= 134, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9242 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A294896_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static seq_t A294896_sigma(idx_t n)
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

static seq_t A294896_binw(idx_t n)
{
    seq_t result = 0;
    seq_t power = (seq_t)n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A294896_gcd(A294896_sigma(n), A294896_binw(n));
}
