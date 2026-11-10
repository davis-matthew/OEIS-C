/*
 * A324293  a(n) = A002487(sigma(n)).
 * https://oeis.org/A324293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(sigma(n)); sigma(n) by trial-division
 *            factorization, Stern's diatomic series by binary iteration
 *            (s(2k)=s(k), s(2k+1)=s(k)+s(k+1))
 * Range:     u8 n <= 495, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9887 (2026-11-08)
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

static seq_t A002487_a(seq_t n)
{
    seq_t x = 1, y = 0;
    while (n > 0) {
        if ((n & 1) != 0) y += x;
        else x += y;
        n >>= 1;
    }
    return y;
}

seq_t a(idx_t n)
{
    return A002487_a(A000203_a(n));
}
