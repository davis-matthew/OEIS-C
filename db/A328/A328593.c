/*
 * A328593  Numbers whose binary indices have no consecutive divisible parts.
 * https://oeis.org/A328593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose binary indices have no consecutive divisible parts */
#include <stdint.h>
#include <stdbool.h>

/* Return true if the binary indices of x contain no pair of consecutive
   indices i<j such that i divides j. */
static inline bool has_good_indices(idx_t x)
{
    idx_t m = x;
    int prev = 0;
    while (m) {
        /* Position of the lowest set bit, counting from 1. */
        int pos = __builtin_ctzll((unsigned long long)m) + 1;
        if (prev && pos % prev == 0)
            return false;
        prev = pos;
        m &= m - 1;           /* clear lowest set bit */
    }
    return true;
}

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
