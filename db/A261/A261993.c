/*
 * A261993  Number of distinct fractional parts of the numbers
 *          1/(prime(j)-1)+...+1/(prime(k)-1) with 1 <= j <= k <= n, where the
 *          fractional part of x is given by x - floor(x).
 * https://oeis.org/A261993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 for 6 ≤ n ≤ 17 a(n) = n*(n-1)/2 - 2 for n ≥ 18
 *            a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=11
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Small explicit values for 1 .. 5 */
    static const seq_t small[5] = {1, 2, 4, 7, 11};

    if (n <= 5)
        return small[n - 1];

    seq_t sn = (seq_t)n;
    seq_t res = sn * (sn - 1) / 2;

    if (n >= 18)
        res -= 2;

    return res;
}
