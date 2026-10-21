/*
 * A289610  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 317) or the same sequence
 *          for the mesh patterns (12, 377), (12, 407), (12, 467).
 * https://oeis.org/A289610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) - 2^(n-2) - n + 2 for n > 1, a(0) = 1, a(1) =
 *            1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    if (n == 1u) return 1;

    seq_t catal = catalan(n);          /* Catalan number C(n)  */
    seq_t pow2_val = pow2(n - 2);      /* 2^(n-2)              */
    seq_t n_seq = (seq_t)n;            /* cast index to seq_t   */

    /* a(n) = C(n) - 2^(n-2) - n + 2  */
    seq_t result = catal - pow2_val;
    result -= n_seq;
    result += 2;

    return result;
}
