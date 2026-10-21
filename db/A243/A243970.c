/*
 * A243970  Smallest positive integer m such that n can be expressed as a
 *          partial sum of the divisors of m taken in decreasing order.
 * https://oeis.org/A243970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Smallest positive integer m such that n can be expressed
 *            as a partial sum of the divisors of m taken in decreasing order
 * Range:     u8 n <= 255, u16 n <= 9851, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   zero_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/zero_array.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)             /* a(0) = 1 by definition */
        return (seq_t)1;

    /* Allocate a temporary array for answers up to n. */
    seq_t *ans = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!ans) return (seq_t)0;     /* safety: out‑of‑memory */

    ans[0] = (seq_t)1;             /* a(0) value, never used for n>0 */
    idx_t filled = 0;              /* how many non‑zero entries < n */

    for (idx_t m = 1; m <= n && filled < n; ++m)
    {
        /* Enumerate divisors of m in two arrays: large (descending)
           and small (ascending). */
        idx_t small[256];
        idx_t large[256];
        size_t sz_small = 0;
        size_t sz_large = 0;

        for (idx_t k = 1; k <= m / k; ++k)
        {
            if (m % k == 0)
            {
                small[sz_small++] = k;
                if (k != m / k)
                    large[sz_large++] = m / k;
            }
        }

        /* Walk the divisors in decreasing order and record
           the first m that reaches each partial sum. */
        seq_t sum = (seq_t)0;

        /* Large divisors first (they are strictly decreasing). */
        for (size_t i = 0; i < sz_large; ++i)
        {
            sum += (seq_t)large[i];
            idx_t idx = (idx_t)sum;
            if (idx <= n && ans[idx] == (seq_t)0)
            {
                ans[idx] = (seq_t)m;
                ++filled;
            }
        }

        /* Small divisors in reverse order (now decreasing). */
        for (size_t i = sz_small; i-- > 0; )
        {
            sum += (seq_t)small[i];
            idx_t idx = (idx_t)sum;
            if (idx <= n && ans[idx] == (seq_t)0)
            {
                ans[idx] = (seq_t)m;
                ++filled;
            }
        }
    }

    seq_t res = ans[n];
    free(ans);
    return res;
}
