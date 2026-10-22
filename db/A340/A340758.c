/*
 * A340758  Number of partitions of n into 4 parts with the same number of
 *          distinct prime factors.
 * https://oeis.org/A340758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊n/4⌋} Sum_{j=k..⌊(n−k)/3⌋}
 *            Sum_{i=j..⌊(n−j−k)/2⌋} [ Ω(k)=Ω(j)=Ω(i)=Ω(n−i−j−k) ] where Ω is
 *            the number of distinct prime factors
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation pre‑computes the Ω values up to the largest n
    requested, groups numbers by Ω, then uses a combinatorial DP to
    count unordered 4‑part partitions composed of numbers from the same
    group.  The function is memoised so that repeated calls with larger
    n only extend the precomputation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t Nmax = 0;
    static seq_t *answers = NULL;     /* a[0..Nmax] */

    /* extend precomputation if needed */
    if (!answers || n > Nmax)
    {
        idx_t newN = n;
        /* allocate answer array */
        seq_t *newAns  = (seq_t *)calloc((size_t)(newN + 1), sizeof(seq_t));
        if (!newAns) abort();

        /* 1. compute Ω(k) for 1..newN */
        uint8_t *omega = (uint8_t *)calloc((size_t)(newN + 1), sizeof(uint8_t));
        if (!omega) abort();

        for (idx_t p = 2; p <= newN; ++p)
            if (omega[p] == 0)          /* p is prime */
                for (idx_t m = p; m <= newN; m += p)
                    ++omega[m];

        /* 2. determine maximum Ω value */
        uint8_t maxOmega = 0;
        for (idx_t i = 2; i <= newN; ++i)
            if (omega[i] > maxOmega) maxOmega = omega[i];

        /* 3. build lists of numbers sharing the same Ω */
        size_t  maxO     = (size_t)maxOmega;
        uint32_t **lists = (uint32_t **)calloc(maxO + 1, sizeof(uint32_t *));
        size_t   *counts = (size_t *)calloc(maxO + 1, sizeof(size_t));
        size_t   *caps   = (size_t *)calloc(maxO + 1, sizeof(size_t));
        if (!lists || !counts || !caps) abort();

        /* Ω(1)=0, initialise its list */
        lists[0] = (uint32_t *)malloc(sizeof(uint32_t));
        if (!lists[0]) abort();
        lists[0][0] = 1;
        counts[0] = 1;
        caps[0]   = 1;

        for (idx_t i = 2; i <= newN; ++i)
        {
            uint8_t t = omega[i];
            if (counts[t] >= caps[t])
            {
                size_t newCap = caps[t] ? caps[t] * 2 : 4;
                uint32_t *tmp = (uint32_t *)realloc(lists[t], newCap * sizeof(uint32_t));
                if (!tmp) abort();
                lists[t] = tmp;
                caps[t] = newCap;
            }
            lists[t][counts[t]++] = (uint32_t)i;
        }
        free(omega);

        /* 4. DP for each Ω value */
        size_t sumSize = (size_t)(newN + 1);
        for (size_t t = 0; t <= maxO; ++t)
        {
            if (counts[t] == 0) continue;

            /* dp[used][sum] flattened */
            seq_t *dp = (seq_t *)calloc(5 * sumSize, sizeof(seq_t));
            if (!dp) abort();
            dp[0] = (seq_t)1;        /* 0 parts sum to 0 */

            for (size_t idx = 0; idx < counts[t]; ++idx)
            {
                uint32_t v = lists[t][idx];
                for (size_t used = 1; used <= 4; ++used)
                {
                    size_t baseUsed = used * sumSize;
                    size_t basePrev = (used - 1) * sumSize;
                    for (size_t s = v; s <= newN; ++s)
                        dp[baseUsed + s] += dp[basePrev + (s - v)];
                }
            }

            /* add 4‑part counts to answer array */
            size_t base4 = 4 * sumSize;
            for (size_t s = 0; s <= newN; ++s)
                newAns[s] += dp[base4 + s];

            free(dp);
        }

        /* free auxiliary structures */
        for (size_t t = 0; t <= maxO; ++t)
            free(lists[t]);
        free(lists);
        free(counts);
        free(caps);

        /* replace old table */
        free(answers);
        answers = newAns;
        Nmax = newN;
    }

    return answers[n];
}
