/*
 * A286632  Base-3 digit sum of A254103: a(n) = A053735(A254103(n)).
 * https://oeis.org/A286632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A053735(A254103(n)); A254103(0)=0,
 *            A254103(2n)=3*A254103(n)-1,
 *            A254103(2n+1)=floor(3*(1+A254103(n))/2)
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 137 terms through n = 8172 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t v = 0;
    int started = 0;
    for (int i = 63; i >= 0; --i) {
        int bit = (int)((n >> i) & 1);
        if (!started) {
            if (!bit) continue;
            started = 1;
        }
        if (bit)
            v = ((seq_t)3 * (v + 1)) / 2;
        else
            v = (seq_t)3 * v - 1;
    }
    seq_t sum = 0;
    for (seq_t x = v; x != 0; x /= 3)
        sum += x % 3;
    return sum;
}
