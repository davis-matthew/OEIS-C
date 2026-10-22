/*
 * A328593  Numbers whose binary indices have no consecutive divisible parts.
 * https://oeis.org/A328593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   has_good_indices
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose binary indices have no consecutive divisible parts */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_good_indices.h"

/* a(n) – the nth term of OEIS A328593 (offset 1).  */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset 1, behaviour below offset is unused */
        return (seq_t)0;

    idx_t count = 0;
    for (idx_t cur = 0; ; ++cur) {
        if (has_good_indices(cur)) {
            ++count;
            if (count == n)
                return (seq_t)cur;
        }
    }
}
