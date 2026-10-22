/*
 * A386011  Total number of inversions in all parking functions of length n.
 * https://oeis.org/A386011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2) * n*(n+1)^(n-2)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t res = binomial(n, 2);          /* binomial(n,2) = n*(n-1)/2 */
    res = res * (seq_t)n;                /* multiply by n */
    seq_t pow = big_pow((seq_t)n + 1, (seq_t)n - 2); /* (n+1)^(n-2) */
    res = res * pow;                     /* multiply by the power */
    res = res / 2;                       /* divide by 2 */
    return res;
}
