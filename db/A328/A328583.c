/*
 * A328583  a(n) = A023900(A276086(n)).
 * https://oeis.org/A328583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Product_{k>=0, d_k(n)>0} (1 - prime(k+1)), where d_k(n)
 *            are the digits of n in primorial base (a(n) =
 *            A023900(A276086(n)); a(0) = 1)
 * Range:     i8 n <= 245, i16 n <= 32585, i32 n <= 32768, i64 n <= 32768,
 *            i128 n <= 32768, big any n
 * Validated: standard, pass, 96 terms through n = 9582 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A328583_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    for (;;) {
        int ok = 1;
        for (uint64_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = 0; break; }
        }
        if (ok) return c;
        c++;
    }
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p = 2;
    while (m > 0) {
        if (m % p != 0)
            result *= ((seq_t)1 - (seq_t)p);
        m /= p;
        p = A328583_next_prime(p);
    }
    return result;
}
