/*
 * A337300  Partial sums of the geometric Connell sequence A049039.
 * https://oeis.org/A337300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - k*(n+1) + 2*(2^k - 1), where k = floor(log2(n))
 * Range:     u8 n <= 17, u16 n <= 259, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   my_floor_log2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/my_floor_log2.h"

seq_t a(idx_t n)
{
    /* for n == 0 return 0 (offset is 1, but this is harmless) */
    if (n == 0)
        return (seq_t)0;

    unsigned k = my_floor_log2(n);
    seq_t n_seq = (seq_t)n;
    seq_t pow2k = ((seq_t)1) << k;                 /* 2^k */
    seq_t term1 = n_seq * n_seq;                   /* n^2 */
    seq_t term2 = (seq_t)k * (n_seq + (seq_t)1);   /* k*(n+1) */
    seq_t term3 = (pow2k << 1) - (seq_t)2;         /* 2*(2^k - 1) */
    return term1 - term2 + term3;
}
