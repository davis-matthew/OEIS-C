/*
 * A347271  Irregular triangle T(n,k) read by rows in which row n lists the
 *          terms of the 3x+1 trajectory of n, but the row ends when a term is
 *          a power of 2 or when a term is less than n, with n >= 1 and k >=
 *          0.
 * https://oeis.org/A347271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 3x+1 trajectory rows ending at a power of 2 or a term <
 *            n, read row by row
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; this case is unused */

    idx_t target = n;
    idx_t count = 0;                       /* number of terms produced so far */
    idx_t start = 1;                       /* current row number */

    for ( ;; ++start )
    {
        seq_t cur = (seq_t)start;          /* first term of the row */
        ++count;
        if (count == target) return cur;

        /* if the starting term is already a power of 2, the row ends here */
        if (is_power_of_two(cur)) continue;

        /* generate the rest of the row */
        while (1)
        {
            if ((cur & 1) == 0)            /* even: divide by 2 */
                cur = cur >> 1;
            else                            /* odd: 3x + 1 */
                cur = cur * 3 + 1;

            ++count;
            if (count == target) return cur;

            /* stop if we hit a power of 2 or a term less than the row start */
            if (is_power_of_two(cur) || cur < (seq_t)start)
                break;
        }
    }
}
