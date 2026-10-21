/*
 * A254332  Indices of centered pentagonal numbers (A005891) which are also
 *          squares (A000290).
 * https://oeis.org/A254332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 38*a(n-2) - 38*a(3) - a(n-4) + a(n-5), a(1) = 1,
 *            a(2) = 3, a(3) = 22, a(4) = 96, a(5) = 817 (see OEIS A254332)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but harmless */

    /* initial values */
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)3;
    seq_t a3 = (seq_t)22;
    seq_t a4 = (seq_t)96;
    seq_t a5 = (seq_t)817;

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;
    if (n == 5) return a5;

    idx_t i;
    for (i = 6; i <= n; ++i) {
        seq_t next = a5
                   + (seq_t)38 * a4
                   - (seq_t)38 * a3
                   - a2
                   + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
