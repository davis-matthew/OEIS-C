/*
 * A396036  Growth series of higher Heisenberg group H_7, with standard
 *          generators.
 * https://oeis.org/A396036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = sum_{k=0}^{8} B_k * binom(n,k) with B =
 *            [1,27,365,3331,22123,112605,444281,1364891,3244921]
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const seq_t B[9] = {
        (seq_t)1,     (seq_t)27,      (seq_t)365,
        (seq_t)3331,  (seq_t)22123,   (seq_t)112605,
        (seq_t)444281, (seq_t)1364891, (seq_t)3244921
    };
    seq_t res = 0;
    idx_t limit = n < 9 ? n : 8;
    for (idx_t k = 0; k <= limit; ++k)
    {
        res += B[k] * binom(n, k);
    }
    return res;
}
