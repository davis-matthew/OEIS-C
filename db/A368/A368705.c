/*
 * A368705  Number of squarefree numbers that are less than n, not dividing n,
 *          and relatively prime to n.
 * https://oeis.org/A368705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} mu(k)^2 * floor(1/gcd(n,k)) *
 *            (ceil(n/k)-floor(n/k))
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t count = 0;
    seq_t sn = (seq_t)n;

    for (idx_t k = 2; k < n; ++k) {
        if (!is_squarefree((seq_t)k))
            continue;
        seq_t sk = (seq_t)k;
        if (gcd(sn, sk) != 1)
            continue;
        count++;
    }
    return count;
}
