/*
 * A292600  a(n) = A006068(floor(n/2)); A006068 with every term duplicated,
 *          where A006068 is the inverse of binary gray code.
 * https://oeis.org/A292600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A006068(floor(n/2)), where A006068(k) is the inverse of
 *            binary Gray code: gray(original) = original xor (original >> 1),
 *            and inverse_gray(g) satisfies inverse_gray(g) xor
 *            (inverse_gray(g) >> 1) = g. Thus a(n) returns the binary inverse
 *            Gray code of floor(n/2)
 * Range:     u8 n <= 511, u16 n <= 9021, u32 n <= 9021, u64 n <= 9021,
 *            u128 n <= 9021, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* take floor(n/2) and compute its binary inverse Gray code */
    seq_t g = (seq_t)(n >> 1);     /* Gray code value */
    for (seq_t mask = g >> 1; mask != (seq_t)0; mask >>= 1)
        g ^= mask;                /* binary inverse */
    return g;
}
