/*
 * A277700  a(n) = A000120(A264977(n)); number of odd terms on row n of
 *          A125184.
 * https://oeis.org/A277700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000120(A264977(n)), with A264977(0)=0, A264977(1)=1,
 *            A264977(2n)=2*A264977(n), A264977(2n+1)=A264977(n) XOR
 *            A264977(n+1)
 * Range:     u8 n <= 8193, u16 n <= 8193, u32 n <= 8193, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Validated: standard, pass, 137 terms through n = 7762 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* keep the pair (x, y) = (A264977(m), A264977(m+1)), where m = the bits of n read so far */
    unsigned __int128 x = 0, y = 1;
    int top = 63;
    while (top >= 0 && !((n >> top) & 1ULL)) top--;
    for (int i = top; i >= 0; i--) {
        if ((n >> i) & 1ULL) {
            unsigned __int128 nx = x ^ y;
            unsigned __int128 ny = y << 1;
            x = nx; y = ny;
        } else {
            unsigned __int128 nx = x << 1;
            unsigned __int128 ny = x ^ y;
            x = nx; y = ny;
        }
    }
    seq_t c = 0;
    while (x) {
        c += (seq_t)(unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}
