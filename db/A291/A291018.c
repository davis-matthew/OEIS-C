/*
 * A291018  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - 6 S + S^2.
 * https://oeis.org/A291018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=6, a(1)=41, a(2)=280; a(n) = 8*a(n-1) - 8*a(n-2) for n >= 3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0u) return 6;
    if (n == 1u) return 41;
    if (n == 2u) return 280;
    seq_t a1 = 41, a2 = 280, an = 0;
    idx_t i;
    for (i = 3; i <= n; ++i) {
        an = 8 * a2 - 8 * a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
