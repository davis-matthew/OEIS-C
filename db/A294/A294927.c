/*
 * A294927  Number of proper divisors of n that are nondeficient (A023196).
 * https://oeis.org/A294927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors d of n that are nondeficient
 *            (σ(d) ≥ 2·d)
 * Range:     u8 n <= 8543, u16 n <= 9560, u32 n <= 9560, u64 n <= 9560,
 *            u128 n <= 9560, big any n
 * Helpers:   is_nondeficient, sum_of_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_nondeficient.h"
#include "../utils/sum_of_divisors.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0, a(0) undefined */

    seq_t cnt = 0;
    idx_t limit = n;

    for (idx_t i = 1; i * i <= limit; ++i)
    {
        if (limit % i == 0)
        {
            /* first divisor */
            if (i < limit && is_nondeficient(i))
                ++cnt;

            idx_t j = limit / i;
            if (j != i && j < limit && is_nondeficient(j))
                ++cnt;
        }
    }

    return cnt;
}
