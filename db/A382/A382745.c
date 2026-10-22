/*
 * A382745  If k appears, 7*k does not.
 * https://oeis.org/A382745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth positive integer whose exponent of 7 in its prime
 *            factorization is even (including 0)
 * Range:     u8 n <= 224, u16 n <= 8751, u32 n <= 8751, u64 n <= 8751,
 *            u128 n <= 8751, big any n
 * Helpers:   even_cnt_u7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/even_cnt_u7.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1; return 0 for safety */
        return (seq_t)0;

    idx_t low  = 1;
    /* Upper bound: a(n) < 1.15·n for all n, add a safety margin. */
    idx_t high = n + n / 7 + 10;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (even_cnt_u7(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
