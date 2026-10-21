/*
 * A254942  Number of length n 1..(3+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A254942 – Number of length‑n 1..4 arrays with every leading partial sum
    divisible by 2, 3, 5, 7 or 11.  The code uses a DP over residues
    modulo the lcm(2,3,5,7,11)=2310.  The result for a(n) is produced
    by iterating the DP until n steps, caching all intermediate values
    so that calls with arbitrary (non‑monotone) indices are fast.  */

#include <stdlib.h>
#include <stdbool.h>

#define MOD 2310          /* lcm(2,3,5,7,11) */

seq_t a(idx_t n)
{
    /* Arrays for dynamic programming over residues modulo MOD. */
    static seq_t dp[MOD];
    static seq_t next[MOD];

    /* Boolean table telling whether a residue is acceptable. */
    static bool okResid[MOD];

    /* Cache of computed a(k) values. */
    static seq_t *sums = NULL;   /* sums[0] = a(1), sums[1] = a(2), ... */
    static size_t sumSize = 0, sumCap = 0;

    /* Number of steps already processed. */
    static idx_t prevN = 0;   /* we have prepared values up to a(prevN). */

    /* One‑time static initialization. */
    static bool init = false;
    if (!init)
    {
        /* Precompute acceptable residues. */
        for (idx_t r = 0; r < MOD; ++r)
            okResid[r] = ((r % 2) == 0) || ((r % 3) == 0) ||
                         ((r % 5) == 0) || ((r % 7) == 0) ||
                         ((r % 11) == 0);

        /* Start with an empty prefix sum: residue 0 has one way. */
        for (idx_t i = 0; i < MOD; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        init = true;
    }

    if (n == 0)      /* undefined in OEIS – return zero for safety. */
        return (seq_t)0;

    /* Advance the DP until we have computed a(n). */
    while (prevN < n)
    {
        /* Clear next array. */
        for (idx_t i = 0; i < MOD; ++i)
            next[i] = (seq_t)0;

        /* Transition from the previous step. */
        for (idx_t cur = 0; cur < MOD; ++cur)
        {
            seq_t ways = dp[cur];
            if (ways == (seq_t)0)
                continue;

            /* Try all possible entries 1..4. */
            for (idx_t a = 1; a <= 4; ++a)
            {
                idx_t newr = (cur + a) % MOD;
                if (!okResid[newr])
                    continue;
                next[newr] += ways;  /* wrap‑around on overflow. */
            }
        }

        /* Sum of all ways after this step gives a(prevN+1). */
        seq_t total = (seq_t)0;
        for (idx_t r = 0; r < MOD; ++r)
            total += next[r];

        /* Store the computed value. */
        if (sumSize == sumCap)
        {
            size_t newCap = sumCap ? sumCap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(sums, newCap * sizeof(seq_t));
            if (!tmp)   /* allocation failed – let the program abort. */
                return (seq_t)0;
            sums = tmp;
            sumCap = newCap;
        }
        sums[sumSize++] = total;

        /* Prepare for the next iteration. */
        for (idx_t r = 0; r < MOD; ++r)
            dp[r] = next[r];
        ++prevN;
    }

    return sums[n - 1];
}
