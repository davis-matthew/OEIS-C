/*
 * A246860  Expected value of trace(O)^(2n), where O is a 4 X 4 orthogonal
 *          matrix randomly selected according to Haar measure.
 * https://oeis.org/A246860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n)*(Catalan(n)+1)/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset starts at 1 */
    seq_t cat = catalan(n);
    return cat * (cat + 1) / 2;
}
