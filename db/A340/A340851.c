/*
 * A340851  Number of factorizations of n such that every factor is a divisor
 *          of the number of factors.
 * https://oeis.org/A340851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of multiset factorizations of n into factors >1,
 *            where each factor divides the total number of factors (k). For
 *            n=1 the empty factorization counts as 1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   dfs_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/dfs_k.h"

seq_t a(idx_t n)
{
    if (n == 1)              /* the empty factorization */
        return 1;

    seq_t ans = 0;

    /* maximum k satisfies 2^k <= n, so k <= floor(log2 n) */
    idx_t max_k = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++max_k;
    }

    for (idx_t k = 2; k <= max_k; ++k) {
        /* collect proper divisors of k (all >1) */
        idx_t divs[32];
        size_t divcnt = 0;
        for (idx_t d = 2; d <= k; ++d) {
            if (k % d == 0)
                divs[divcnt++] = d;
        }
        if (divcnt == 0)
            continue;

        ans += dfs_k((seq_t)n, 0, divs, divcnt, 0, k);
    }
    return ans;
}
