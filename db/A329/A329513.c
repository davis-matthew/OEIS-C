/*
 * A329513  G.f. = (1+x)^2*(1+2*x^2-x^3)/(1-x).
 * https://oeis.org/A329513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(3)=9, a(4)=9, a(n≥5)=8 (from the
 *            generating function)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t first[5] = {(seq_t)1, (seq_t)3, (seq_t)6, (seq_t)9, (seq_t)9};
    if (n < 5)
        return first[(size_t)n];
    return (seq_t)8;
}
