/*
 * A340599  Number of factorizations of n into factors > 1 with length and
 *          greatest factor equal.
 * https://oeis.org/A340599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into factors > 1 where the
 *            number of factors equals the greatest factor
 * Range:     u8 n <= 9947, u16 n <= 9947, u32 n <= 9947, u64 n <= 9947,
 *            u128 n <= 9947, big any n
 * Helpers:   count_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_exact.h"

seq_t a(idx_t n)
{
    /* Sum over all possible lengths L with 2 ≤ L and 2^L ≤ n. */
    seq_t result = 0;
    for (idx_t L = 2; ; ++L) {
        if (((idx_t)1 << L) > n) break;
        seq_t cnt_max = count_exact(L, n, 2, L);        /* max factor ≤ L   */
        seq_t cnt_less = count_exact(L, n, 2, L - 1);   /* max factor ≤ L-1 */
        result += cnt_max - cnt_less;
    }
    return result;
}
