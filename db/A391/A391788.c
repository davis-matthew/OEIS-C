/*
 * A391788  Array read by antidiagonals: A(n,k) = phi(n+k) with k >= 0 and
 *          A(0,0) = 0.
 * https://oeis.org/A391788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = φ( ⌊(√(8·n + 1) − 1)/2⌋ )
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   diag_for_idx, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the totient routine from utils/phi.h */

#include "../utils/phi.h"
#include "../utils/diag_for_idx.h"

/* external function required by the build system */
seq_t a(idx_t n)
{
    idx_t m = diag_for_idx(n);
    if (m == 0) return 0;   /* φ(0) is defined as 0 in the OEIS table  */
    return phi(m);
}
