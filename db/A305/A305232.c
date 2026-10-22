/*
 * A305232  Number of ordered ways to write 2*n+1 as p + binomial(2k,k) +
 *          2*binomial(2m,m), where p is an odd prime, and k and m are
 *          nonnegative integers.
 * https://oeis.org/A305232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered representations of 2*n+1 as p +
 *            choose(2k,k) + 2*choose(2m,m), where p is an odd prime, and k,m
 *            are non‑negative integers
 * Range:     u8 n <= 3, u16 n <= 7739, u32 n <= 9914, u64 n <= 9914,
 *            u128 n <= 9914, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t central_binomial(idx_t k, seq_t limit)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply then divide – division is exact each step */
        res = res * (k + i) / i;
        if (res > limit)  /* no need to compute further */
            return limit + 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n * 2 + 1;          /* 2*n + 1 */
    seq_t count  = 0;

    for (idx_t k = 0; ; ++k)
    {
        seq_t cbk = central_binomial(k, target);
        if (cbk > target)
            break;                           /* further k will be larger */

        for (idx_t m = 0; ; ++m)
        {
            seq_t cbm = central_binomial(m, target);
            if (cbm > target)
                break;                       /* further m will be larger */

            seq_t term = target - cbk - (cbm * 2);
            if (term <= 1)
                continue;
            if (((term & 1) == 1) && is_prime_big(term))
                ++count;
        }
    }

    return count;
}
