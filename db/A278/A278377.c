/*
 * A278377  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          determinant n with no elements repeated.
 * https://oeis.org/A278377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×2 matrices with all entries distinct in
 *            {0,…,n} and determinant equal to n. The implementation counts
 *            unordered pairs of unordered pairs of distinct elements with
 *            product difference n, subtracts the overlapping configurations,
 *            and multiplies by 4 to account for element orderings
 * Range:     u8 n <= 13, u16 n <= 155, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2)                 /* trivial cases */
        return 0;

    /* Number of unordered distinct pairs (x<y) from 0..n */
    const idx_t N = n;
    const idx_t maxprod = N * N;                /* <= 1e8 for n≤1e4 */

    /* Allocate array for counts of unordered pairs with a given product.
       With n≤1e4, uint16_t is sufficient for the maximum count. */
    uint16_t *cnt = (uint16_t *)calloc((size_t)(maxprod + 1), sizeof(uint16_t));
    if (!cnt)                 /* allocation failed */
        return 0;

    /* Fill cnt[p] = number of unordered distinct pairs (x<y) with x*y = p */
    for (idx_t a = 0; a <= N; ++a)
    {
        for (idx_t d = a + 1; d <= N; ++d)
        {
            idx_t prod = a * d;
            ++cnt[prod];
        }
    }

    /* Sum over unordered pairs (a<d), counting unordered pairs (b,c) with
       product ad - n.  Each such unordered pair-of-unordered-pairs is
       counted exactly once. */
    seq_t sum_all = 0;
    for (idx_t a = 0; a <= N; ++a)
    {
        for (idx_t d = a + 1; d <= N; ++d)
        {
            idx_t prod = a * d;
            if (prod < n)
                continue;                 /* product too small */
            sum_all += (seq_t)cnt[prod - n];
        }
    }

    /* Count configurations where the two unordered pairs share one element.
       For each common element c dividing n, let d = n / c.  The number of
       valid v such that (c, v) and (c, v+d) are unordered pairs with
       distinct elements and v+d ≤ n.  Adjust for forbidden v = c or v = c-d. */
    seq_t overlap = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* first divisor */
            idx_t c1 = d;
            idx_t d1 = n / c1;
            {   /* process c1 */
                idx_t max_v = n - d1;
                if (max_v < (idx_t)-1)          /* works because max_v is unsigned */
                {
                    seq_t cntv = (seq_t)max_v + 1;
                    if (c1 <= max_v) cntv--;
                    if (c1 >= d1 && (c1 - d1) <= max_v) cntv--;
                    if (cntv > 0)
                        overlap += cntv;
                }
            }

            /* second divisor, skip if equal to the first */
            if (c1 != d1)
            {
                idx_t c2 = d1;
                idx_t d2 = n / c2;   /* equals c1 */
                {   /* process c2 */
                    idx_t max_v = n - d2;
                    seq_t cntv = (seq_t)max_v + 1;
                    if (c2 <= max_v) cntv--;
                    if (c2 >= d2 && (c2 - d2) <= max_v) cntv--;
                    if (cntv > 0)
                        overlap += cntv;
                }
            }
        }
    }

    /* Final answer: each unordered pair-of-unordered-pairs gives 4 distinct matrices. */
    seq_t result = 4 * (sum_all - overlap);

    free(cnt);
    return result;
}
