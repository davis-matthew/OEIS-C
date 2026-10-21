/*
 * A244415  Exponent of 4 appearing in the 4-adic value of 1/n, n >= 1, given
 *          in A240226(n).
 * https://oeis.org/A244415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = valuation(2*n,4) (equivalently floor((v2(n)+1)/2))
 * Range:     u8 n <= 8804, u16 n <= 8804, u32 n <= 8804, u64 n <= 8804,
 *            u128 n <= 8804, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    if (n & 1)  /* n is odd */
        return (seq_t)0;

    idx_t v = 0;
    /* Count the number of trailing zeros in n (v2(n)) */
    while ((n & 1) == 0) {
        ++v;
        n >>= 1;
    }
    /* Result is ceil(v/2) = (v+1)/2 for even n */
    return (seq_t)((v + 1) / 2);
}
