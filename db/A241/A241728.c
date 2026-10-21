/*
 * A241728  Number of compositions of n into distinct parts with exactly nine
 *          descents.
 * https://oeis.org/A241728
 *
 * Offset:    55    (a(55) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s≥10} Eulerian(s,9) * P(n−s(s−1)/2, s) where P(m, s)
 *            is the number of partitions of m into exactly s parts (order
 *            does not matter). For a composition of n into distinct parts
 *            with exactly nine descents, we first choose a set S of distinct
 *            parts of size s whose sum is n. Any permutatio...
 * Range:     u8 n <= 65, u16 n <= 75, u32 n <= 102, u64 n <= 198,
 *            u128 n <= 569, big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"   /* for seq_alloc_new_array */

/* forward declaration of seq_t and idx_t are provided by the build */
extern uint64_t _floor_isqrt_u64(idx_t n);

/* maximum n expected in the tests */
#define MAX_N   10000
/* maximum parts needed for a(n) up to MAX_N (s(s+1)/2 ≤ MAX_N)  */
#define MAX_K   141        /* 141*(141+1)/2 = 10091 > 10000 */

/* global static data used after first call */
static bool initialized = false;
static seq_t **parts;                /* DP table for P(m,s)   (m ≤ MAX_N, s ≤ MAX_K) */
static seq_t euler[MAX_K + 1][10];   /* Eulerian numbers A(n, m) for m=0..9 */

/* initialize DP tables */
static void init_tables(void)
{
    size_t i, j;

    /* allocate DP table for partitions of m into exactly s parts */
    parts = (seq_t **)malloc((MAX_N + 1) * sizeof(seq_t *));
    for (i = 0; i <= MAX_N; ++i)
    {
        parts[i] = seq_alloc_new_array(MAX_K + 1);  /* create a row of length MAX_K+1 */
        memset(parts[i], 0, (MAX_K + 1) * sizeof(seq_t));
    }

    /* base: P(0,0) = 1 */
    parts[0][0] = 1;

    /* fill DP table */
    for (i = 1; i <= MAX_N; ++i)
    {
        for (j = 1; j <= MAX_K; ++j)
        {
            seq_t val = parts[i - 1][j - 1];
            if (i >= j)
                val += parts[i - j][j];
            parts[i][j] = val;
        }
    }

    /* compute Eulerian numbers up to n=MAX_K and m=9 */
    memset(euler, 0, sizeof(euler));
    for (j = 0; j <= MAX_K; ++j)
        euler[j][0] = 1;        /* A(n,0) = 1 for all n */

    for (i = 1; i <= MAX_K; ++i)
    {
        for (j = 1; j <= 9; ++j)
        {
            if (j > i)
                break;           /* A(n,m)=0 for m>n */
            seq_t term1 = (seq_t)(i - j) * euler[i - 1][j - 1];
            seq_t term2 = (seq_t)(j + 1) * euler[i - 1][j];
            euler[i][j] = term1 + term2;
        }
    }

    initialized = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!initialized)
        init_tables();

    /* n is guaranteed to be ≥ 55 by the problem statement */
    int s_min = 10;                    /* need at least 10 parts to have 9 descents */
    int s_max = 0;                     /* largest m with m(m+1)/2 ≤ n */
    while ((s_max + 1) * (s_max + 2) / 2 <= (int)n)
        ++s_max;

    if (s_max > MAX_K)
        s_max = MAX_K;                 /* safety for unexpected larger n */

    seq_t total = 0;

    for (int s = s_min; s <= s_max; ++s)
    {
        int m = (int)n - s * (s - 1) / 2;
        if (m < 0)
            continue;                  /* cannot happen due to s_max selection */

        seq_t pdistinct = parts[m][s];
        if (pdistinct == 0)
            continue;

        seq_t e = euler[s][9];
        if (e == 0)
            continue;

        total += pdistinct * e;
    }

    return total;
}
