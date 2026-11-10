/*
 * A341055  Inverse permutation to A341054.
 * https://oeis.org/A341055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = inverse permutation of A341054; write n in balanced
 *            ternary (d_1..d_k), then a(n) has digits t_m = d_m - d_{m-1} mod
 *            3 (d_0 = 0), with 2 read as -1
 * Range:     u8 n <= 121, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Validated: standard, pass, 78 terms through n = 9820 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    int dig[70];
    int len = 0;
    idx_t t = n;
    while (t > 0) {
        int r = (int)(t % 3);
        t /= 3;
        if (r == 2) {
            dig[len++] = -1;
            t += 1;
        } else {
            dig[len++] = r;
        }
    }
    seq_t result = 0;
    int prev = 0;
    for (int i = len - 1; i >= 0; --i) {
        int diff = (dig[i] - prev) % 3;
        if (diff < 0) diff += 3;
        prev = dig[i];
        result = result * 3;
        if (diff == 2) result = result - 1;
        else result = result + diff;
    }
    return result;
}
