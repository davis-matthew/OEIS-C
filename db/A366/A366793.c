/*
 * A366793  Binary encoding of the ones in the balanced ternary representation
 *          of Per Nørgård's "infinity sequence".
 * https://oeis.org/A366793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A289813(A323909(n)): write A004718(n) in balanced
 *            ternary, and set bit k of a(n) when the digit at position k is
 *            +1. A004718(0)=0, A004718(2k)=-A004718(k),
 *            A004718(2k+1)=A004718(k)+1
 * Range:     u8 n <= 65536, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 122 terms through n = 9471 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    long long x = 0;
    int msb = -1;
    idx_t t = n;
    while (t > 0) { msb++; t >>= 1; }
    for (int i = msb; i >= 0; i--) {
        if ((n >> i) & 1) x += 1;
        else x = -x;
    }

    seq_t result = 0;
    int k = 0;
    while (x != 0) {
        int r = (int)(x % 3);
        x /= 3;
        if (r == 2) { r = -1; x += 1; }
        else if (r == -2) { r = 1; x -= 1; }
        if (r == 1) result |= ((seq_t)1 << k);
        k++;
    }
    return result;
}
