/*
 * A372672  a(n) = phi(10 * n)/4.
 * https://oeis.org/A372672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = phi(10*n)/4
 * Range:     u8 n <= 25, u16 n <= 7807, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* a(0) is undefined; return 0 for safety */
    }
    idx_t m = (idx_t)10 * n;     /* m = 10 * n, computed in idx_t to avoid overflow */
    seq_t phi_val = phi(m);      /* phi(10*n) */
    return phi_val / (seq_t)4;   /* divide by 4, result fits in seq_t */
}
