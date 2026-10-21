/*
 * A294739  Numbers that are the sum of 5 nonzero squares in exactly 5 ways.
 * https://oeis.org/A294739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerates all integers n ≤ 5408 that can be expressed
   as the sum of five non‑zero squares in exactly five distinct
   non‑ordered ways. The algorithm uses a dynamic programming
   routine that counts unordered multisets of squares with repetition
   allowed. */

/* Type names are supplied by the build environment. */
#include <stdlib.h>

#define MAX_N 5408          /* largest n that may occur in the sequence */

static seq_t *terms = NULL;      /* array of sequence values, allocated once */
static size_t term_count = 0;     /* number of values stored */

/* ----------------------------------------------------------------------
 * Initialise the sequence data structure on first use.
 * ---------------------------------------------------------------------- */
static void init_sequence(void)
{
    static int already_done = 0;
    if (already_done) return;
    already_done = 1;

    /* dp[used][sum] = number of unordered multisets of exactly
       'used' squares that sum to 'sum'.  The squares are all
       non‑zero squares ≤ MAX_N. */
    seq_t dp[6][MAX_N + 1];
    for (int u = 0; u < 6; ++u)
        for (int s = 0; s <= MAX_N; ++s)
            dp[u][s] = 0;

    dp[0][0] = 1;   /* base case: 0 squares sum to 0 */

    /* iterate over all non–zero squares s^2 up to MAX_N */
    for (int s = 1; s * s <= MAX_N; ++s) {
        int sq = s * s;
        /* allow unlimited use of this square by iterating 'used' in ascending
           order.  This ensures that each multiset is counted once. */
        for (int sum = sq; sum <= MAX_N; ++sum) {
            for (int used = 1; used <= 5; ++used) {
                dp[used][sum] += dp[used - 1][sum - sq];
            }
        }
    }

    /* collect all numbers that have exactly five representations */
    size_t cap = 32;
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    for (int n = 1; n <= MAX_N; ++n) {
        if (dp[5][n] == 5) {
            if (term_count == cap) {
                cap <<= 1;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[term_count++] = (seq_t)n;
        }
    }
}

/* ----------------------------------------------------------------------
 * Public entry point: return the nth term (1‑based indexing).
 * If n exceeds the number of known terms, return 0.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined case, defensive */
    init_sequence();
    if ((idx_t)n > term_count) return 0;
    return terms[n - 1];
}
