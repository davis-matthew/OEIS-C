/*
 * A358244  Number of n-regular, N_0-weighted pseudographs on 2 vertices with
 *          total edge weight 4, up to isomorphism.
 * https://oeis.org/A358244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (30*n + 3*(-1)^n - 73)/2 for n ≥ 6 Initial values:
 *            a(1)=1, a(2)=6, a(3)=13, a(4)=27, a(5)=38 (The recurrence
 *            a(n)=a(n-1)+a(n-2)-a(n-3) holds for n≥9.)
 * Range:     u8 n <= 11, u16 n <= 3406, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 1 … 5 */
    static const seq_t init[5] = {1, 6, 13, 27, 38};

    if (n < 6)
        return init[(size_t)(n - 1)];

    seq_t nn = (seq_t)n;
    /* numerator = 30*n - 70 for even n, 30*n - 76 for odd n */
    seq_t num = (seq_t)30 * nn;
    if (n & 1ULL)           /* n is odd */
        num = num - (seq_t)76;
    else                    /* n is even */
        num = num - (seq_t)70;

    return num / (seq_t)2;
}
