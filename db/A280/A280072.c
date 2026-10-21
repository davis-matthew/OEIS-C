/*
 * A280072  Indices of centered 11-gonal numbers (A060544) that are also
 *          11-gonal numbers (A051682).
 * https://oeis.org/A280072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 21*a(n-2) + a(n-3) for n>3, a(1)=1, a(2)=11,
 *            a(3)=210
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 11;
    if (n == 3) return 210;

    seq_t a1 = 1;
    seq_t a2 = 11;
    seq_t a3 = 210;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1   = 21 * a3;
        seq_t t2   = 21 * a2;
        seq_t next = t1 - t2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
