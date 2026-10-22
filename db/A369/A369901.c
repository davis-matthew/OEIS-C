/*
 * A369901  Proth numbers h*2^k+1, with odd h < 2^k, ordered first by k then
 *          by h.
 * https://oeis.org/A369901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = (2*(n - 2^⌊log2(n)⌋) + 1) * 2^(⌊log2(n)⌋+1) + 1 Computes
 *            OEIS A369901: Proth numbers h*2^k+1 with odd h < 2^k, ordered
 *            first by k then by h
 * Range:     u8 n <= 19, u16 n <= 319, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* find b = floor(log2(n)) and c = n - 2^b */
    idx_t b = floor_log2_uint64(n);
    idx_t pow2b = ((idx_t)1) << b;          /* 2^b */
    idx_t c = n - pow2b;                    /* n - 2^b */

    seq_t term    = ((seq_t)2 * c) + 1;     /* 2c+1 */
    seq_t pow2b1  = pow2(b + 1);            /* 2^(b+1) */

    return term * pow2b1 + 1;
}
