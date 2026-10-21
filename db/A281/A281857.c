/*
 * A281857  Numbers occurring in a curious cubic identity.
 * https://oeis.org/A281857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1000^n - 100^n + 2*10^n - 2)/6 (n ≥ 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute the three powers of 10, 100, 1000 */
    seq_t p1000 = pow_seq((seq_t)1000, (int)n);
    seq_t p100  = pow_seq((seq_t)100,  (int)n);
    seq_t p10   = pow_seq((seq_t)10,   (int)n);

    /* (1000^n - 100^n + 2*10^n - 2) / 6 */
    seq_t num = p1000 - p100 + p10 * 2 - 2;
    return num / 6;
}
