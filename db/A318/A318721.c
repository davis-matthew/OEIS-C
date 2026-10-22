/*
 * A318721  Number of strict relatively prime factorizations of n.
 * https://oeis.org/A318721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strict relatively prime factorizations of n
 *            (unordered, with distinct factors >1, and gcd of all factors =
 *            1)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static void dfs(idx_t rem, idx_t start, seq_t gprev, seq_t *cnt)
{
    for (idx_t f = start; f <= rem / f; ++f)
    {
        if (rem % f != 0)
            continue;
        idx_t rest = rem / f;
        seq_t gnew = gcd(gprev, (seq_t)f);

        if (rest >= f + 1)          /* distinct and ascending order */
        {
            /* Count the factorization where 'rest' is the last factor */
            seq_t gfinal = gcd(gnew, (seq_t)rest);
            if (gfinal == (seq_t)1)
                ++(*cnt);

            /* Recurse to split the remaining part further */
            dfs(rest, f + 1, gnew, cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;         /* as per OEIS offset */

    seq_t count = 0;
    dfs(n, 2, (seq_t)0, &count);
    return count;
}
