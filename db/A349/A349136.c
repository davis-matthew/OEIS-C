/*
 * A349136  Möbius transform of Kimberling's paraphrase, A003602.
 * https://oeis.org/A349136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n even; a(1)=1; for odd n>1, a(n)=phi(n)/2
 * Range:     u8 n <= 446, u16 n <= 9057, u32 n <= 9057, u64 n <= 9057,
 *            u128 n <= 9057, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    if ((n & (idx_t)1) == 0) {
        return (seq_t)0;
    }
    /* n is odd > 1 */
    seq_t ph = phi(n);
    return ph / 2;
}
