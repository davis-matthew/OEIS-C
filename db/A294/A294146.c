/*
 * A294146  Sum of the larger parts of the partitions of n into two parts with
 *          smaller part squarefree.
 * https://oeis.org/A294146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n - i) * mu(i)^2 where mu(i)^2 is
 *            1 iff i is squarefree, else 0
 * Range:     u8 n <= 31, u16 n <= 532, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Test if x is squarefree by trial division. */
static bool is_squarefree(idx_t x)
{
    if (x <= 1) return true;          /* 1 is squarefree */
    idx_t t = x;
    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            int exp = 0;
            while (t % p == 0)
            {
                t /= p;
                exp++;
                if (exp > 1) return false;  /* found a squared factor */
            }
        }
    }
    return true;
}

/* OEIS A294146 */
seq_t a(idx_t n)
{
    idx_t half = n / 2;
    seq_t sum = 0;
    for (idx_t i = 1; i <= half; ++i)
    {
        if (is_squarefree(i))
            sum += (seq_t)(n - i);
    }
    return sum;
}
