/*
 * A355297  a(n) = A007088(n) mod n.
 * https://oeis.org/A355297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(n) mod n, where A007088(n) is the binary
 *            expansion of n read as a decimal number; computed by Horner's
 *            rule modulo n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    unsigned __int128 r = 0;
    unsigned __int128 m = (unsigned __int128)n;
    int top = 63;
    while (top >= 0 && !((n >> top) & 1)) top--;
    for (int i = top; i >= 0; i--) {
        r = r * 10 + (unsigned)((n >> i) & 1);
        r %= m;
    }
    return (seq_t)(uint64_t)r;
}
