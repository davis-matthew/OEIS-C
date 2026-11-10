/*
 * A324965  Partial sums of A324964.
 * https://oeis.org/A324965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A324964(k) = number of m<=n with m = 4j+1, j
 *            having no adjacent 1s in binary; computed by a Zeckendorf-style
 *            digit count
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    uint64_t J = (uint64_t)((n - 1) / 4);
    uint64_t c[66];
    c[0] = 1;
    c[1] = 2;
    for (int i = 2; i < 64; i++)
        c[i] = c[i - 1] + c[i - 2];
    uint64_t cnt = 0;
    int prev = 0;
    int stopped = 0;
    for (int i = 63; i >= 0; i--) {
        int bit = (int)((J >> i) & 1);
        if (bit) {
            cnt += c[i];
            if (prev) {
                stopped = 1;
                break;
            }
        }
        prev = bit;
    }
    if (!stopped)
        cnt += 1;
    return (seq_t)cnt;
}
