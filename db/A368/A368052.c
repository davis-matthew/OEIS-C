/*
 * A368052  The degree of polynomials related to Somos-4 sequences. Also for n
 *          > 2 the degree of the (n-3)-th involution in a family of
 *          involutions in the Cremona group of rank 3 defined by a Somos-4
 *          sequence.
 * https://oeis.org/A368052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + e(n-2)+e(n-1)+e(n)+e(n+1) * Recurrence: a(n)
 *            -3a(n-1)+3a(n-2)-a(n-3)+a(n-4)-3a(n-5)+3a(n-6)-a(n-7) = b_n *
 *            with b_4=2, b_5=-3, b_6=2, else 0. For n>=7 we have the
 *            homogeneous * linear recurrence of order 7: * a(n) = 3a(n-1)
 *            -3a(n-2) + a(n-3) - a(n-4) + 3a(n-5) -3a(n-6) + a(n-7)
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values a(0)..a(7) */
    static const seq_t init[8] = { 0, 0, 0, 0, 2, 3, 5, 8 };

    if (n < 8) return init[n];

    seq_t buf[8];
    for (unsigned i = 0; i < 8; ++i) buf[i] = init[i];

    for (idx_t k = 8; k <= n; ++k) {
        seq_t next = 3 * buf[7];
        next -= 3 * buf[6];
        next += buf[5];
        next -= buf[4];
        next += 3 * buf[3];
        next -= 3 * buf[2];
        next += buf[1];

        /* shift buffer left by one position */
        for (unsigned i = 0; i < 7; ++i) buf[i] = buf[i + 1];
        buf[7] = next;
    }
    return buf[7];
}
