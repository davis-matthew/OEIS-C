/*
 * A304720  Number of nonnegative integers k such that n - (4^k - k) is
 *          positive and squarefree.
 * https://oeis.org/A304720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k ≥ 0 | n > (4^k - k) and n - (4^k - k) is squarefree}
 * Range:     u8 n <= 398, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 116 terms through n = 9167 (2026-11-01)
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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
