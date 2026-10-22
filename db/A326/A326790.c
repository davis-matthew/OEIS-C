/*
 * A326790  The rank of the group of functions on the units of Z/nZ generated
 *          by the functions f(u) = u*k mod n.
 * https://oeis.org/A326790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1, otherwise a(n) = floor((φ(n) + 2)/2)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here φ(n) is Euler's totient function. */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;

    seq_t phi = calc_phi((seq_t)n);
    return (phi + (seq_t)2) / (seq_t)2;
}
