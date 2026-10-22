/*
 * A344319  Number of partitions of n into consecutive parts not divisible by
 *          4.
 * https://oeis.org/A344319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of intervals [L,R] with L%4≠0, R%4≠0 and
 *            sum_{k=L..R, k%4≠0} k == n. (Each such interval gives a distinct
 *            partition of n into consecutive parts not divisible by 4.)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not part of the offset, safe guard */

    seq_t count = 0;

    for (idx_t L = 1; L <= n; ++L)
    {
        if (L % 4 == 0) continue;       /* L must be a part (not divisible by 4) */

        seq_t sum = 0;
        for (idx_t R = L; R <= n; ++R)
        {
            if (R % 4 != 0)
                sum += (seq_t)R;

            if (sum > (seq_t)n)
                break;                   /* further R can only increase the sum */

            if (R % 4 != 0 && sum == (seq_t)n)
                ++count;                 /* valid partition */
        }
    }

    return count;
}
