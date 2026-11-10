/*
 * A265337  Number of 1-bits in base-3 reversal of n: a(n) =
 *          A000120(A263273(n)).
 * https://oeis.org/A265337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000120(A263273(n)); A263273(n) = base-3 reversal of n
 *            (trailing zeros kept in place), a(0) = 0
 * Range:     u8 n <= 1000, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 145 terms through n = 9696 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t temp = n;
    seq_t p3 = (seq_t)1;
    while ((temp % (idx_t)3) == 0) {
        temp /= (idx_t)3;
        p3 *= (seq_t)3;
    }
    seq_t rev = (seq_t)0;
    idx_t m = temp;
    while (m > 0) {
        rev = rev * (seq_t)3 + (seq_t)(m % (idx_t)3);
        m /= (idx_t)3;
    }
    rev = rev * p3;
    seq_t c = 0;
    while (rev != 0) {
        c += (seq_t)(rev & 1);
        rev >>= 1;
    }
    return c;
}
