/*
 * A394938  a(n) = floor(log_2(2*n - 1)) + floor(log_2(2*n - 1 -
 *          2^(floor(log_2(2*n - 2)) - 1))) + 2.
 * https://oeis.org/A394938
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log_2(2*n - 1)) + floor(log_2(2*n - 1 -
 *            2^(floor(log_2(2*n - 2)) - 1))) + 2
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   floor_log2_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_log2_seq.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                           /* undefined below offset */
    seq_t two_n  = ((seq_t)n << 1);                /* 2*n   */
    seq_t t1     = two_n - 1;                      /* 2*n-1 */
    unsigned a1  = floor_log2_seq(t1);             /* floor(log2(2*n-1)) */

    seq_t t2_base = two_n - 2;                     /* 2*n-2 */
    unsigned l2  = floor_log2_seq(t2_base);        /* floor(log2(2*n-2)) */

    seq_t pow_val = ((seq_t)1 << (l2 - 1));        /* 2^(floor(log2(2*n-2))-1) */
    seq_t t2     = t1 - pow_val;                   /* 2*n-1 - 2^(...) */
    unsigned a2  = floor_log2_seq(t2);             /* floor(log2(2*n-1-2^(...))) */

    seq_t res = (seq_t)a1 + (seq_t)a2 + 2;
    return res;
}
