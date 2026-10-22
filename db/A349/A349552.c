/*
 * A349552  a(n) is the number of halving partitions of n (see Comments for
 *          definition).
 * https://oeis.org/A349552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of halving partitions of n (recursive)
 * Range:     u8 n <= 5290, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   count_halving
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_halving.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t fl = n / 2;
    idx_t ce = (n + 1) / 2;
    seq_t total = 0;

    if (fl > 0 && fl <= n)
        total += count_halving(n - fl, fl);

    if (ce > 0 && ce <= n && ce != fl)
        total += count_halving(n - ce, ce);

    return total;
}
