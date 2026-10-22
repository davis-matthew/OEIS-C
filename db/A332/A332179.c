/*
 * A332179  a(n) = 7*(10^(2*n+1) - 1)/9 + 2*10^n.
 * https://oeis.org/A332179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*(10^(2*n+1) - 1)/9 + 2*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    if (n == 0) return 9;
    if (n == 1) return 797;
    if (n == 2) return 77977;

    seq_t a0 = 9, a1 = 797, a2 = 77977;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 111 * a2 - 1110 * a1 + 1000 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
