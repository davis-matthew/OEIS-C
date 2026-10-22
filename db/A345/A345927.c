/*
 * A345927  Alternating sum of the binary expansion of n (row n of A030190).
 *          Replace 2^k with (-1)^(A070939(n)-k) in the binary expansion of n
 *          (compare to the definition of A065359).
 * https://oeis.org/A345927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(floor(log2(n))) * Σ_{k≥0} bit_k(n) * (-1)^k where
 *            bit_k(n) is the k‑th bit of n (LSB at k = 0). a(0) = 0
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    unsigned len = floor_log2_uint64(n);   /* A070939(n) = floor(log2(n)) */
    seq_t sum = 0;
    idx_t tmp = n;
    unsigned bit = 0;

    while (tmp) {
        if (tmp & 1) {
            if (bit & 1)   /* odd bit position: sign is -1 */
                sum -= 1;
            else           /* even bit position: sign is +1 */
                sum += 1;
        }
        tmp >>= 1;
        ++bit;
    }

    if (len & 1)          /* (-1)^len is -1 when len is odd */
        sum = -sum;

    return sum;
}
