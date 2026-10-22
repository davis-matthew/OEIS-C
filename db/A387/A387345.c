/*
 * A387345  Coefficient of x^n in the expansion of ( (1-x+x^4) / (1-x) )^n.
 * https://oeis.org/A387345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n,k) * binomial(n-3*k-1,
 *            n-4*k)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 32, u64 n <= 61, u128 n <= 118,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                      /* a(0) = 1 */

    seq_t sum = 0;
    idx_t maxk = n >> 2;                /* floor(n/4) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binom(n, k);         /* binomial(n,k) */

        /* compute the second binomial factor */
        idx_t f = n - 3 * k - 1;        /* n - 3k - 1 */
        idx_t g = n - 4 * k;            /* n - 4k */

        /* For n>0 the values stay non‑negative; binom handles 0 <= g <= f */
        seq_t t2 = binom(f, g);         /* binomial(f,g) */

        sum += t1 * t2;
    }

    return sum;
}
