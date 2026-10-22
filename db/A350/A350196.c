/*
 * A350196  a(n) is the smallest positive integer which can be represented as
 *          the sum of distinct nonzero pentagonal numbers in exactly n ways,
 *          or 0 if no such integer exists.
 * https://oeis.org/A350196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the smallest positive integer that can be expressed
   as a sum of distinct nonzero pentagonal numbers in exactly n ways. */

#include <stdbool.h>
#include <stdint.h>

/* Configuration constants – adjust if you want a larger search. */
#define MAX_SUM   200000   /* maximum sum to explore                          */
#define MAX_COUNT 10000    /* we only keep counts up to 10 000               */

/* Tables holding counts and the minimal sums for each count.  All
   indices and intermediate counts are kept in the built‑in integral
   type idx_t; the returned value is coerced to seq_t. */
static idx_t dp[MAX_SUM + 1];
static idx_t minSumForCount[MAX_COUNT + 1];
static bool initialized = false;

/* One‑time initialisation routine. */
static void init(void)
{
    /* 0th sum is obtainable in exactly one way (the empty sum). */
    for (idx_t i = 0; i <= MAX_SUM; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Reset the answer array. */
    for (idx_t c = 1; c <= MAX_COUNT; ++c)
        minSumForCount[c] = 0;

    /* Enumerate pentagonal numbers and update the DP table. */
    for (idx_t k = 1;; ++k)
    {
        /* p_k = k(3k-1)/2 */
        idx_t p = k * ((idx_t)3 * k - 1) / 2;
        if (p > MAX_SUM)
            break;

        for (idx_t i = MAX_SUM; i >= p; --i)
        {
            idx_t add = dp[i] + dp[i - p];
            if (add > (idx_t)(MAX_COUNT + 1))
                add = (idx_t)(MAX_COUNT + 1);   /* cap counts beyond MAX_COUNT */
            dp[i] = add;
        }
    }

    /* Convert the DP table to a lookup of the minimal sum for each count. */
    for (idx_t i = 1; i <= MAX_SUM; ++i)
    {
        idx_t cnt = dp[i];
        if (cnt == 0 || cnt > MAX_COUNT)
            continue;
        if (minSumForCount[cnt] == 0)
            minSumForCount[cnt] = i;   /* first (smallest) occurrence */
    }

    initialized = true;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n == 0 || n > MAX_COUNT)
        return (seq_t)0;   /* undefined or out of range */

    return (seq_t)minSumForCount[n];
}
