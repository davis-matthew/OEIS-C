/*
 * A338246  Nonpositive values in A117966, in order of appearance and negated.
 * https://oeis.org/A338246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; a(n) = -A117966(A157671(n)) for n > 0, where
 *            A157671(n) = (3^ceil(log_3(2n)) + 2n - 1)/2. Computed digit by
 *            digit: in the ternary expansion of A157671(n), digit 2
 *            contributes +3^i and digit 1 contributes -3^i
 * Range:     u8 n <= 147, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Validated: standard, pass, 84 terms through n = 9815 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t value = n * 2;
    idx_t power = 1;
    while (power < value) power *= 3;
    idx_t m = (power + value - 1) / 2;

    seq_t pos = 0, neg = 0, p = 1;
    while (m > 0) {
        int d = (int)(m % 3);
        if (d == 2) pos += p;
        else if (d == 1) neg += p;
        p *= 3;
        m /= 3;
    }
    return pos - neg;
}
