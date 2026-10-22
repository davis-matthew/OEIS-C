/*
 * A304720  Number of nonnegative integers k such that n - (4^k - k) is
 *          positive and squarefree.
 * https://oeis.org/A304720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k ≥ 0 | n > (4^k - k) and n - (4^k - k) is squarefree}
 * Range:     u8 n <= 398, u16 n <= 9167, u32 n <= 9167, u64 n <= 9167,
 *            u128 n <= 9167, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t val = (seq_t)1;           /* 4^0 */
    idx_t k = 0;
    seq_t count = 0;

    while (1) {
        seq_t term = val - (seq_t)k;   /* 4^k - k */
        if (term >= N)
            break;
        seq_t m = N - term;           /* n - (4^k - k) */
        if (is_squarefree(m))
            ++count;
        val *= 4;                      /* next power of 4 */
        ++k;
    }
    return count;
}
