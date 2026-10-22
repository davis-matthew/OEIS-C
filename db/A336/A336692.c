/*
 * A336692  Binary weight of 1+sigma(n).
 * https://oeis.org/A336692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 1‐bits in (1 + sigma(n)) (OEIS A336692)
 * Range:     u8 n <= 1001, u16 n <= 9065, u32 n <= 9065, u64 n <= 9065,
 *            u128 n <= 9065, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

static inline seq_t popcount_seq(seq_t x)
{
    seq_t cnt = 0;
    while (x)
    {
        cnt++;
        x &= (x - 1);
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);
    s += 1;            /* add 1 before taking binary weight */
    return popcount_seq(s);
}
