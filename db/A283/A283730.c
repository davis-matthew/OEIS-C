/*
 * A283730  Number of 5Xn 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of 5×n binary arrays in which exactly one 1 is
 * allowed to be equal to more than two of its horizontal, diagonal and
 * antidiagonal neighbors.  The algorithm uses a DP over columns with a
 * sliding‑window of three columns. */

#include <stdbool.h>
#include <stddef.h>

 /* Precompute how many violating 1s occur in a middle column given its
  * left and right neighbours.  bad[left][mid][right] is 0,1 or 2
  * (2 means ≥2 violations). */
static unsigned char bad[32][32][32];

/* --------- helper to count violations in a middle column --------- */
static unsigned char count_violations(unsigned char mid, unsigned char left,
                                      unsigned char right)
{
    unsigned char viol = 0;
    for (int r = 0; r < 5; ++r)
    {
        if ((mid >> r) & 1)
        {
            unsigned char nb = 0;
            /* horizontal neighbours */
            nb += (left  >> r) & 1;
            nb += (right >> r) & 1;
            /* diagonal and antidiagonal neighbours */
            if (r > 0)
                nb += (left  >> (r - 1)) & 1;   /* up‑left  */
            if (r < 4)
                nb += (right >> (r + 1)) & 1;   /* down‑right */
            if (r < 4)
                nb += (left  >> (r + 1)) & 1;   /* down‑left  */
            if (r > 0)
                nb += (right >> (r - 1)) & 1;   /* up‑right   */
            if (nb > 2)
                ++viol;
        }
    }
    return viol;
}

/* Precompute the bad table once. */
static __attribute__((constructor)) void init_bad_table(void)
{
    for (unsigned char left = 0; left < 32; ++left)
        for (unsigned char mid = 0; mid < 32; ++mid)
            for (unsigned char right = 0; right < 32; ++right)
            {
                unsigned char v = count_violations(mid, left, right);
                bad[left][mid][right] = v >= 2 ? 2 : v;
            }
}

/* ----------------------------------------------------------------- */
/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* DP arrays: dp[left][right][viol], nextdp similar. */
    seq_t dp[32][32][2]  = {0};
    seq_t nextdp[32][32][2] = {0};

    dp[0][0][0] = 1;          /* start with two virtual zero columns */

    /* Main DP loop over columns */
    for (idx_t step = 0; step < n; ++step)
    {
        /* zero nextdp */
        for (int l = 0; l < 32; ++l)
            for (int r = 0; r < 32; ++r)
                nextdp[l][r][0] = nextdp[l][r][1] = 0;

        for (int left = 0; left < 32; ++left)
            for (int mid = 0; mid < 32; ++mid)
            {
                seq_t v0 = dp[left][mid][0];
                seq_t v1 = dp[left][mid][1];
                if (!v0 && !v1)
                    continue;

                for (int right = 0; right < 32; ++right)
                {
                    unsigned char add = bad[left][mid][right];

                    if (add == 0)
                    {
                        if (v0)
                            nextdp[mid][right][0] += v0;
                        if (v1)
                            nextdp[mid][right][1] += v1;
                    }
                    else if (add == 1)
                    {
                        if (v0)
                            nextdp[mid][right][1] += v0;
                        /* v1 would produce 2 violations – skip */
                    }
                    /* add == 2 : skip all transitions for this right */
                }
            }

        /* swap dp and nextdp */
        for (int l = 0; l < 32; ++l)
            for (int r = 0; r < 32; ++r)
            {
                dp[l][r][0] = nextdp[l][r][0];
                dp[l][r][1] = nextdp[l][r][1];
            }
    }

    /* Final evaluation of the last column (right neighbour is 0). */
    seq_t ans = 0;
    for (int left = 0; left < 32; ++left)
        for (int mid = 0; mid < 32; ++mid)
            for (int w = 0; w <= 1; ++w)
            {
                seq_t cnt = dp[left][mid][w];
                if (!cnt)
                    continue;
                unsigned char add = bad[left][mid][0];   /* right=0 */
                if (w + add == 1)
                    ans += cnt;
            }

    return ans;
}
