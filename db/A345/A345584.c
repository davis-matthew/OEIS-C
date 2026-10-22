/*
 * A345584  Numbers that are the sum of eight fourth powers in nine or more
 *          ways.
 * https://oeis.org/A345584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3053, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are the sum of eight fourth powers in nine or more ways. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Global buffers for the dynamic programming tables and the solution list. */
static uint32_t *dp[9] = {0};     /* dp[parts][sum] – number of
                                    non‑decreasing 8‑tuples with the given sum. */
static seq_t *solutions = NULL;   /* List of numbers with at least nine
                                    representations. */
static size_t solutions_len = 0;  /* Number of solutions found. */
static size_t solutions_cap = 0;  /* Capacity of the solutions array. */

/* Append a value to the solutions vector, growing it when needed. */
static void append_solution(seq_t v)
{
    if (solutions_len == solutions_cap)
    {
        size_t new_cap = solutions_cap ? solutions_cap << 1 : 64;
        seq_t *ptr = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
        if (!ptr) exit(EXIT_FAILURE);
        solutions = ptr;
        solutions_cap = new_cap;
    }
    solutions[solutions_len++] = v;
}

/* Free the dynamic programming tables. */
static void free_dp(void)
{
    for (int i = 0; i <= 8; ++i)
    {
        if (dp[i]) {
            free(dp[i]);
            dp[i] = NULL;
        }
    }
}

/* Recompute the solution list for all sums up to NEW_MAX inclusive. */
static void recompute(size_t new_max)
{
    free_dp();                                     /* discard old tables */

    /* Allocate new tables.  Use calloc to initialise to zero. */
    for (int i = 0; i <= 8; ++i)
    {
        dp[i] = (uint32_t *)calloc(new_max + 1, sizeof(uint32_t));
        if (!dp[i]) exit(EXIT_FAILURE);
    }
    dp[0][0] = 1;                                  /* base case */

    /* Enumerate each possible fourth‑power value. */
    for (uint64_t v = 1;; ++v)
    {
        uint64_t val64 = v * v * v * v;            /* v^4, fits in 64 bits */
        if (val64 > new_max) break;
        size_t val = (size_t)val64;

        /* Update counts for 1…8 parts using this value. */
        for (int part = 1; part <= 8; ++part)
        {
            for (size_t sum = val; sum <= new_max; ++sum)
            {
                dp[part][sum] += dp[part - 1][sum - val];
            }
        }
    }

    /* Prepare the solutions vector. */
    if (solutions) {
        free(solutions);
        solutions = NULL;
    }
    solutions_len = 0;
    solutions_cap = 64;
    solutions = (seq_t *)malloc(solutions_cap * sizeof(seq_t));
    if (!solutions) exit(EXIT_FAILURE);

    /* Collect all sums that occur in at least nine different ways. */
    for (size_t sum = 0; sum <= new_max; ++sum)
    {
        if (dp[8][sum] >= 9U) {
            append_solution((seq_t)sum);
        }
    }

    free_dp();                                     /* tables no longer needed */
}

/* Ensure we have at least N solutions ready for answering a(n). */
static void ensure_computed(idx_t n)
{
    if (n == 0) return;
    if ((idx_t)solutions_len >= n) return;

    /* Start with a modest range and double it until enough solutions are found. */
    size_t cur_max = 2000000;     /* 2 × 10⁶ as a starting point */
    while ((idx_t)solutions_len < n)
    {
        recompute(cur_max);
        if ((idx_t)solutions_len >= n) break;
        if (cur_max > SIZE_MAX / 2) break;      /* prevent overflow */
        cur_max <<= 1;
    }
}

/* Public interface required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_computed(n);
    if ((idx_t)solutions_len < n) return 0;      /* safety fallback */
    return solutions[n - 1];
}
