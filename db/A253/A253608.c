/*
 * A253608  The binary representation of a(n) is the concatenation of n and
 *          the binary complement of n, A035327(n).
 * https://oeis.org/A253608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n+1) * (2^BL(n) - 1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where BL(n) is the binary length of n, i.e., floor(log2(n)) + 1 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour below offset is irrelevant */
    /* Binary length of n */
    unsigned len = floor_log2_uint64(n) + 1;
    /* (2^len - 1) */
    seq_t pow_minus_one = ((seq_t)1 << len) - 1;
    /* (n + 1) */
    seq_t m = (seq_t)n + 1;
    /* Result */
    return m * pow_minus_one;
}
