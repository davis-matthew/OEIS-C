/*
 * A375959  Partial products of A006257.
 * https://oeis.org/A375959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1..n} (2*(k-2^{⌊log2 k⌋}) + 1), a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the partial products of A006257 for the OEIS sequence A375959. */
/* Uses the helper from "../utils/floor_log2_uint64.h". */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        idx_t k     = floor_log2_uint64(i);          /* ⌊log2 i⌋ */
        idx_t pow2  = ((idx_t)1) << k;               /* 2^k */
        idx_t m     = i - pow2;                      /* i - 2^k   */
        seq_t factor = ((seq_t)2 * (seq_t)m + (seq_t)1); /* 2*m + 1 */
        result = result * factor;
    }
    return result;
}
