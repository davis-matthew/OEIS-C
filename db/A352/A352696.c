/*
 * A352696  a(n) = k if the binary representation of k has a 1 (0) exactly
 *          where a 1 in the n-th row of A237048 occurs at an odd (even)
 *          position, reading from left to right.
 * https://oeis.org/A352696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k whose binary digits, read left to right, list the 1's
 *            of row n of A237048 (T(n,j)=1 iff j(j+1)/2 <= n and j | n -
 *            j(j+1)/2): digit 1 if j is odd, 0 if j is even
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t v = 0;
    for (idx_t k = 1; k * (k + 1) / 2 <= n; ++k) {
        idx_t r = n - k * (k + 1) / 2;
        if (r % k == 0) {
            v = v * 2 + (seq_t)(k & 1);
        }
    }
    return v;
}
