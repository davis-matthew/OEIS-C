/*
 * A326291  Number of unsortable factorizations of n.
 * https://oeis.org/A326291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unsortable factorizations of n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------
   Precomputation of smallest and largest prime factors up to a fixed limit.
   ------------------------------------------------------------------------- */
#define LIMIT 200000   /* large enough for n up to ~10 000 */

static uint64_t spf[LIMIT + 1];
static uint64_t minPrimeV[LIMIT + 1];
static uint64_t maxPrimeV[LIMIT + 1];
static bool spf_prepared = false;

static void prepare_spf(void)
{
    if (spf_prepared)
        return;

    /* Build smallest prime factor sieve */
    for (int i = 0; i <= LIMIT; ++i)
        spf[i] = i;

    for (int i = 2; (int64_t)i * i <= LIMIT; ++i)
    {
        if (spf[i] == (uint64_t)i)
        {
            for (int j = i * i; j <= LIMIT; j += i)
                if (spf[j] == (uint64_t)j)
                    spf[j] = i;
        }
    }

    /* Store min and max prime factor for every integer */
    minPrimeV[0] = 0;
    minPrimeV[1] = 0;
    maxPrimeV[0] = 0;
    maxPrimeV[1] = 0;
    for (int i = 2; i <= LIMIT; ++i)
    {
        minPrimeV[i] = spf[i];
        uint64_t t = i;
        uint64_t mx = 0;
        while (t > 1)
        {
            uint64_t p = spf[t];
            if (p > mx)
                mx = p;
            while (t % p == 0)
                t /= p;
        }
        maxPrimeV[i] = mx;
    }

    spf_prepared = true;
}

/* -------------------------------------------------------------------------
   Cache for previously computed results.
   ------------------------------------------------------------------------- */
#define CACHE_SIZE (LIMIT + 1)

static seq_t cache[CACHE_SIZE];
static bool cache_valid[CACHE_SIZE] = { false };

/* -------------------------------------------------------------------------
   Helper to test a single factorization for unsortability.
   ------------------------------------------------------------------------- */
static void test_unsortable(const uint64_t *factors, int k, seq_t *counter)
{
    /* Smallest and largest prime factor arrays for the factors */
    uint64_t mins[20];
    uint64_t maxs[20];

    for (int i = 0; i < k; ++i)
    {
        mins[i] = minPrimeV[factors[i]];
        maxs[i] = maxPrimeV[factors[i]];
    }

    if (k <= 1)
        return;           /* never occurs for our enumeration */

    /* DP over subsets: dp[mask][last] indicates that mask is visited
       ending with factor 'last' and the condition holds for the chain. */
    static unsigned char dp[1 << 13][13];   /* K <= 13 for n <= 200 000 */
    unsigned int totalMask = 1u << k;
    unsigned int fullMask  = totalMask - 1;

    /* Reset DP table */
    for (unsigned int m = 0; m < totalMask; ++m)
        memset(dp[m], 0, k * sizeof(unsigned char));

    /* Initial states */
    for (int i = 0; i < k; ++i)
        dp[1u << i][i] = 1;

    /* Transitions */
    for (unsigned int mask = 1; mask < totalMask; ++mask)
    {
        for (int last = 0; last < k; ++last)
        {
            if (!dp[mask][last])
                continue;
            for (int nxt = 0; nxt < k; ++nxt)
            {
                if (mask & (1u << nxt))
                    continue;
                if (maxs[last] <= mins[nxt])
                    dp[mask | (1u << nxt)][nxt] = 1;
            }
        }
    }

    /* Check if full mask is reachable */
    for (int last = 0; last < k; ++last)
    {
        if (dp[fullMask][last])
            return;      /* sortable */
    }

    /* Unsortable: increment counter */
    (*counter) += 1;
}

/* -------------------------------------------------------------------------
   Recursive enumeration of all unordered factorizations of 'remaining'.
   ------------------------------------------------------------------------- */
static void dfs_factor(uint64_t remaining, uint64_t start, uint64_t *cur,
                       int depth, seq_t *counter)
{
    /* Add 'remaining' as the last factor if this gives at least two factors */
    if (remaining >= start && depth >= 1)
    {
        cur[depth] = remaining;
        test_unsortable(cur, depth + 1, counter);
    }

    /* Try to split further */
    for (uint64_t f = start; f * f <= remaining; ++f)
    {
        if (remaining % f == 0)
        {
            cur[depth] = f;
            dfs_factor(remaining / f, f, cur, depth + 1, counter);
        }
    }
}

/* -------------------------------------------------------------------------
   Public function required by the problem specification.
   ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n > LIMIT)
        return 0;     /* outside precomputation range, return 0 */

    if (!cache_valid[n])
    {
        seq_t result = 0;
        uint64_t cur[20];
        prepare_spf();                 /* ensure prime tables ready */
        dfs_factor(n, 2, cur, 0, &result);  /* start with smallest factor 2 */
        cache[n] = result;
        cache_valid[n] = true;
    }

    return cache[n];
}
