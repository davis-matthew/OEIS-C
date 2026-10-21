/*
 * A286809  Positions of 1 in A286807; complement of A286808.
 * https://oeis.org/A286809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + 2*(n-1) - ⌊log₂(n)⌋
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* The function is defined for n ≥ 1.  For safety we handle n = 0
       by returning 0, though this case should not occur. */
    if (n == 0)
        return (seq_t)0;

    /* floor_log2_uint64 returns ⌊log₂(n)⌋ for n ≥ 1. */
    unsigned log = floor_log2_uint64(n);

    /* Compute a(n) = 3 + 2*(n-1) - ⌊log₂(n)⌋ in seq_t arithmetic. */
    seq_t part = (seq_t)(n - 1) * 2;   /* 2*(n-1) */
    seq_t res   = (seq_t)3 + part - (seq_t)log;
    return res;
}
