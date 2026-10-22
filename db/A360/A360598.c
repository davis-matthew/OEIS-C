/*
 * A360598  Lexicographically earliest sequence of positive integers such that
 *          the ratios between successive terms, { max(a(n), a(n+1)) /
 *          min(a(n), a(n+1)), n > 0 }, are distinct integers.
 * https://oeis.org/A360598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360598: lexicographically earliest sequence of positive integers
   such that the ratios between successive terms, { max(a(n),a(n+1))/min(a(n),a(n+1)) ,
   n>0 }, are distinct integers.
   The sequence is constructed greedily: at each step choose the smallest
   integer that yields an unused ratio. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Storage for the sequence values */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Storage for the used ratios */
#define SMALL_LIMIT 1000000U                /* ratios up to this value can be indexed */
static bool   *small_used = NULL;          /* indices 0..SMALL_LIMIT */
static seq_t *big_ratios = NULL;           /* ratios > SMALL_LIMIT */
static size_t big_len = 0;
static size_t big_cap = 0;

/* Allocate the table for small ratios if needed */
static void init_small_used(void)
{
    if (!small_used)
    {
        small_used = (bool*)calloc(SMALL_LIMIT + 1, sizeof(bool));
        if (!small_used) { /* abort on allocation failure */ exit(1); }
    }
}

/* Append a new element to the sequence array, enlarging it if necessary */
static void seq_append(seq_t val)
{
    if (seq_len == seq_cap)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) { exit(1); }
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* Append a new ratio to the list of used ratios, expanding storage if needed */
static void ratio_append(seq_t r)
{
    if (r <= (seq_t)SMALL_LIMIT)
    {
        init_small_used();
        small_used[(size_t)r] = true;
        return;
    }
    if (big_len == big_cap)
    {
        size_t new_cap = big_cap ? big_cap * 2 : 4;
        seq_t *tmp = (seq_t*)realloc(big_ratios, new_cap * sizeof(seq_t));
        if (!tmp) { exit(1); }
        big_ratios = tmp;
        big_cap    = new_cap;
    }
    big_ratios[big_len++] = r;
}

/* Check whether a ratio has already been used.  The small table is lazily
   allocated on first use. */
static bool ratio_used(seq_t r)
{
    if (r <= (seq_t)SMALL_LIMIT)
    {
        init_small_used();
        return small_used[(size_t)r];
    }
    for (size_t i = 0; i < big_len; ++i)
        if (big_ratios[i] == r) return true;
    return false;
}

/* Main entry point required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for index 0 */

    /* initialise the first element if this is the first call */
    if (seq_len == 0)
    {
        seq_append((seq_t)1);   /* a(1) = 1 */
    }

    /* generate terms until we reach the requested index */
    while (seq_len < (size_t)n)
    {
        seq_t prev = seq_vals[seq_len - 1];
        seq_t cand = (seq_t)1;

        for (;; ++cand)
        {
            /* Compute ratio = max(prev,cand)/min(prev,cand),
               ensuring it is an integer. */
            seq_t min_val = prev < cand ? prev : cand;
            seq_t max_val = prev > cand ? prev : cand;

            if (max_val % min_val != 0) continue;   /* not an integer ratio */

            seq_t r = max_val / min_val;
            if (!ratio_used(r))
            {
                seq_append(cand);   /* store the new term */
                ratio_append(r);    /* mark the ratio as used */
                break;
            }
        }
    }

    return seq_vals[n - 1];
}
