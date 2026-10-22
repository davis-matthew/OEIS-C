/*
 * A352946  a(n) = Sum_{k=0..floor(n/3)} (n-3*k)^k.
 * https://oeis.org/A352946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-3*k)^k
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 37, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t base = (seq_t)n - (seq_t)3 * (seq_t)k;
        seq_t term = pow_seq(base, (int)k);
        sum += term;
    }
    return sum;
}
