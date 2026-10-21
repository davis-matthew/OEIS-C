/*
 * A262992  Sum of the squarefree numbers among the partition parts of n into
 *          two parts.
 * https://oeis.org/A262992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{i=1}^{\lfloor n/2\rfloor} i*mu(i)^2 +
 *            (n-i)*mu(n-i)^2
 * Range:     u8 n <= 29, u16 n <= 463, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t m = n / 2;

    for (idx_t i = 1; i <= m; ++i) {
        seq_t si = (seq_t)i;
        if (is_squarefree(si))
            sum += si;

        seq_t ti = (seq_t)(n - i);
        if (is_squarefree(ti))
            sum += ti;
    }

    return sum;
}
