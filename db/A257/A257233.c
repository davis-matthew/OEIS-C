/*
 * A257233  Multiplicity sequence for the alternating row sums of triangle
 *          A257232.
 * https://oeis.org/A257233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(m) = number of n such that the alternating row sum of triangle
 *            A257232 equals m
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* static data used by the function */
static bool          g_init          = false;
static idx_t         g_last_n        = 0;      /* last n that has been processed */
static seq_t         g_cur_sum       = 0;      /* S(g_last_n)  */
static idx_t         g_cnt_cap       = 0;      /* capacity of g_counts array   */
static seq_t        *g_counts        = NULL;   /* number of occurrences of S */
static idx_t         g_pri_limit     = 0;      /* highest index for which prime array is initialized */
static uint8_t      *g_pri_mark      = NULL;   /* simple prime sieve */

/* ------------------------------------------------------------------ */
/* Ensure that the prime sieve contains information up to 'limit'.    */
static void
ensure_prime_limit (idx_t limit)
{
    if (limit <= g_pri_limit)
        return;

    uint8_t *mark = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!mark)
        return;                       /* allocation failure – let the caller fail */

    for (idx_t i = 0; i <= limit; ++i)
        mark[i] = 1;
    if (limit >= 0) mark[0] = 0;
    if (limit >= 1) mark[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (mark[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                mark[m] = 0;
        }
    }

    free(g_pri_mark);
    g_pri_mark  = mark;
    g_pri_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that the counts array can hold values up to 'min_cap'.      */
static void
ensure_counts_capacity (idx_t min_cap)
{
    if (g_cnt_cap > min_cap)
        return;

    idx_t new_cap = g_cnt_cap ? g_cnt_cap << 1 : 1;
    while (new_cap <= min_cap)
        new_cap <<= 1;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        return;                       /* allocation failure – let the caller fail */

    memset(new_arr, 0, new_cap * sizeof(seq_t));

    if (g_counts) {
        for (idx_t i = 0; i < g_cnt_cap; ++i)
            new_arr[i] = g_counts[i];
        free(g_counts);
    }

    g_counts = new_arr;
    g_cnt_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function computing the multiplicity sequence.                  */
seq_t
a (idx_t n)
{
    if (!g_init) {
        g_init = true;
        g_counts = NULL;
        g_cnt_cap = 0;
        g_pri_mark = NULL;
        g_pri_limit = 0;
    }

    if (n == 0)
        return 0;

    /* Compute values until the current sum exceeds the requested n. */
    while (g_cur_sum <= n) {
        idx_t next = g_last_n + 1;               /* next n to process */

        /* Extend prime sieve if necessary. */
        ensure_prime_limit(next);

        bool is_prime = (next > 1) && g_pri_mark[next];

        /* Update the alternating row sum. */
        if ((next & 1) == 0) {                   /* even */
            g_cur_sum += is_prime;
        } else {                                 /* odd */
            g_cur_sum += 1 - is_prime;
        }

        /* Make sure the counts array can store counts for this sum. */
        ensure_counts_capacity((idx_t)g_cur_sum);

        /* Increment multiplicity count for the current sum. */
        g_counts[(idx_t)g_cur_sum] += 1;

        g_last_n = next;
    }

    /* At this point g_cur_sum > n, so all occurrences of n are counted. */
    return g_counts[(idx_t)n];
}
