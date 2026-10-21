/*
 * A297135  Numbers having a down-first zigzag pattern in base 6; see
 *          Comments.
 * https://oeis.org/A297135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n-th integer whose base-6 representation has no equal
 *            consecutive digits and whose most significant digit is greater
 *            than the second most significant digit (i.e., a down‑first
 *            zigzag)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_down_first_zigzag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_down_first_zigzag.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n == 0; return 0 for safety */

    seq_t candidate = 1;   /* start searching from 1 upwards */
    idx_t count = 0;       /* number of qualifying numbers found so far */

    while (true) {
        if (is_down_first_zigzag(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;       /* move to the next integer */
    }
}
