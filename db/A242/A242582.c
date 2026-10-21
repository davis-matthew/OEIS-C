/*
 * A242582  Number of length n+3 0..6 arrays with no four elements in a row
 *          with pattern aabb (with a!=b) and new values 0..6 introduced in
 *          0..6 order.
 * https://oeis.org/A242582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+3 arrays over 0..6 with new values
 *            introduced in 0..6 order and no 4‑term block of the form a,a,b,b
 *            (a≠b)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   idx_from_vals, zero_dp_full
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/idx_from_vals.h"
#include "../utils/zero_dp_full.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* offset 1 */

    idx_t len = n + 3;                    /* desired array length */

    seq_t dp[2][7][512];
    zero_dp_full(dp);                     /* initialise all cells to 0 */

    unsigned cur = 0, nxt = 1;
    dp[cur][0][idx_from_vals(7, 7, 0)] = 1;   /* first element is 0 */

    for (idx_t pos = 1; pos < len; ++pos)
    {
        /* zero only the next slice before taking transitions */
        for (unsigned mx = 0; mx <= 6; ++mx)
            for (unsigned s = 0; s < 512; ++s)
                dp[nxt][mx][s] = 0;

        for (unsigned mx = 0; mx <= 6; ++mx)
        {
            for (unsigned s = 0; s < 512; ++s)
            {
                seq_t cnt = dp[cur][mx][s];
                if (cnt == 0) continue;

                unsigned a = s / 64;
                unsigned r = s % 64;
                unsigned b = r / 8;
                unsigned c = r % 8;

                /* Transition with values ≤ mx. */
                for (unsigned v = 0; v <= mx; ++v)
                {
                    if (a == b && a != 7 && c == v && c != 7 && a != c)
                        continue;                 /* disallow aabb pattern */
                    unsigned ns = idx_from_vals(b, c, v);
                    dp[nxt][mx][ns] += cnt;
                }

                /* Transition with new value mx+1 if allowed. */
                if (mx < 6)
                {
                    unsigned v = mx + 1;
                    if (!(a == b && a != 7 && c == v && c != 7 && a != c))
                    {
                        unsigned ns = idx_from_vals(b, c, v);
                        dp[nxt][mx + 1][ns] += cnt;
                    }
                }
            }
        }

        /* swap current and next slices */
        unsigned tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all states to obtain the final answer. */
    seq_t total = 0;
    for (unsigned mx = 0; mx <= 6; ++mx)
        for (unsigned s = 0; s < 512; ++s)
            total += dp[cur][mx][s];

    return total;
}
