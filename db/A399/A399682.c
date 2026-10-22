/*
 * A399682  a(n) = smallest squarefree number of the form n*k-1.
 * https://oeis.org/A399682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest squarefree integer of the form n*k - 1 (k ≥ 1)
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* convert n to the working type */
    for (idx_t k = 1; ; ++k) {
        seq_t t = tn * (seq_t)k - 1;
        if (t == 0)   /* skip the zero that occurs only for n = 1, k = 1 */
            continue;
        if (is_squarefree(t))
            return t;
    }
}
