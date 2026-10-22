/*
 * A332168  a(n) = 6*(10^(2*n+1)-1)/9 + 2*10^n.
 * https://oeis.org/A332168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*(10^(2*n+1)-1)/9 + 2*10^n Recurrence: a(0) = 8, a(1) =
 *            686, a(2) = 66866, a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3)
 *            (n ≥ 3)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)686;
    if (n == 2) return (seq_t)66866;

    seq_t a0 = (seq_t)8;
    seq_t a1 = (seq_t)686;
    seq_t a2 = (seq_t)66866;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = (seq_t)111 * a2
                 - (seq_t)1110 * a1
                 + (seq_t)1000 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
