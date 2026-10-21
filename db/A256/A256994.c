/*
 * A256994  a(n) = n + 1 when n <= 3, otherwise a(n) = 2^(n-2) + 3; also
 *          iterates of A005187 starting from a(1) = 2.
 * https://oeis.org/A256994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 for n <= 3; otherwise a(n) = 2^(n-2) + 3 This
 *            satisfies the recurrence a(1) = 2; a(n) = A005187(a(n-1))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)n + 1;        /* 1→2, 2→3, 3→4 */
    return pow2(n - 2) + 3;          /* 2^(n-2) + 3 for n ≥ 4 */
}
