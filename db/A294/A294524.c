/*
 * A294524  Numbers that have a unique partition into a sum of five
 *          nonnegative squares.
 * https://oeis.org/A294524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n-th element of OEIS A294524
   (numbers that have a unique partition into a sum of five nonnegative
   squares).  The algorithm uses a dynamic‑programming table that counts
   unordered partitions of each integer n (0 ≤ n ≤ LIMIT) into exactly
   k (1 ≤ k ≤ 5) positive squares.  A partition is unique for a given n
   iff the total number of unordered partitions into at most five squares
   is exactly one.  Zero (the representation 0^2+0^2+0^2+0^2+0^2) is
   handled separately and placed as the first term.  The table is built
   once on the first call and memoised for subsequent calls.  LIMIT is
   set to 6000, which comfortably exceeds the known bound 5408 beyond
   which no unique representations exist.  All arithmetic is carried out
   in unsigned integers; no floating‑point or standard library math is
   used. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;  /* guard for one‑time precomputation */
static seq_t  *seq  = NULL;        /* array of sequence values   */
static size_t seq_len = 0;         /* current number of elements */

/*---------------------------------------------------------------------*/
/* Precompute the sequence up to the known bound (6000).  This
   function is called lazily from a(). */
static void precompute(void)
{
    /* upper bound: no unique representations for n > 5408 |
       choose 6000 for safety */
    const idx_t LIMIT = 6000U;

    /* maximum square root needed */
    idx_t max_sq = 0;
    while ((max_sq + 1) * (max_sq + 1) <= LIMIT) max_sq++;

    /* dp[t][n]  ,  0 ≤ t ≤ 5 , 0 ≤ n ≤ LIMIT
       use a flat array for efficiency             */
    const size_t rows = 6U;                     /* t = 0..5 */
    const size_t cols = (size_t)(LIMIT + 1U);    /* n   */
    unsigned int *dp = (unsigned int *)calloc(rows * cols, sizeof(unsigned int));
    if (!dp) exit(1);   /* abort on allocation failure */

    /* helper macro to index the 2‑D array */
    #define DP(t, n) dp[(size_t)(t) * cols + (size_t)(n)]

    /* base: 0 can be represented with 0 squares in exactly 1 way */
    DP(0, 0) = 1U;

    /* DP over each possible square value */
    for (idx_t s = 1; s <= max_sq; ++s) {
        const idx_t sq = s * s;
        /* for each number of parts t = 1..5 */
        for (idx_t t = 1; t <= 5U; ++t) {
            /* extend partitions by one part of size sq */
            for (idx_t n = sq; n <= LIMIT; ++n) {
                DP(t, n) += DP(t - 1, n - sq);
            }
        }
    }

    /* Allocate array for the resulting sequence values (including 0). */
    seq = (seq_t *)malloc((size_t)(LIMIT + 1U) * sizeof(seq_t));
    if (!seq) exit(1);
    seq_len = 0U;

    /* 0 is always the first element of the sequence (offset=1). */
    seq[seq_len++] = (seq_t)0U;

    /* For n ≥ 1, count total partitions into at most 5 squares. */
    for (idx_t n = 1; n <= LIMIT; ++n) {
        unsigned int total = 0U;
        for (idx_t t = 1; t <= 5U; ++t) total += DP(t, n);
        if (total == 1U) {
            seq[seq_len++] = (seq_t)n;
        }
    }

    /* Clean up */
    free(dp);
    initialized = true;
    #undef DP
}

/*---------------------------------------------------------------------*/
/* Returned value a(n) is the n-th term of the sequence.
   Arguments:  n ≥ 1, offset 1 (a(1) = 0).                     */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0U;   /* safety; offset is 1 */
    if (!initialized) precompute();
    if ((idx_t)n - 1U < seq_len) return seq[(size_t)n - 1U];
    return (seq_t)0U;                /* out of bounds, should not happen */
}
