/*
 * A305335  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4 a(2) = 5 a(3) = 12 a(4) = 26 a(5) = 62 a(6) = 166 a(7)
 *            = 431 a(8) = 1091 For n > 8: a(n) = a(n-1) + 3*a(n-2) + 3*a(n-3)
 *            + 2*a(n-4) - 3*a(n-5) - 8*a(n-6) - 4*a(n-7)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool, true, false */

/* Maximum n that this implementation supports (adjustable if needed). */
#define MAX_N 20000

static seq_t memo[MAX_N + 1] = {0};
static bool memo_inited = false;
static idx_t memo_max_computed = 0;

/* Initialise the memoisation table with the first eight terms. */
static void init_memo(void) {
    if (!memo_inited) {
        memo[1] = (seq_t)4;
        memo[2] = (seq_t)5;
        memo[3] = (seq_t)12;
        memo[4] = (seq_t)26;
        memo[5] = (seq_t)62;
        memo[6] = (seq_t)166;
        memo[7] = (seq_t)431;
        memo[8] = (seq_t)1091;
        memo_inited = true;
        memo_max_computed = 8;
    }
}

seq_t a(idx_t n) {
    if (n < 1) {
        return (seq_t)0;   /* The sequence is defined for n ≥ 1. */
    }

    /* Ensure memoisation table is initialised. */
    init_memo();

    /* Clamp to the maximum supported index. */
    if (n > (idx_t)MAX_N) {
        return (seq_t)0;
    }

    /* If the value is already computed, return it. */
    if (n <= memo_max_computed) {
        return memo[n];
    }

    /* Compute all missing values up to n using the recurrence. */
    for (idx_t i = memo_max_computed + 1; i <= n; ++i) {
        seq_t term = memo[i-1]
                   + (seq_t)3 * memo[i-2]
                   + (seq_t)3 * memo[i-3]
                   + (seq_t)2 * memo[i-4]
                   - (seq_t)3 * memo[i-5]
                   - (seq_t)8 * memo[i-6]
                   - (seq_t)4 * memo[i-7];
        memo[i] = term;
    }

    memo_max_computed = n;
    return memo[n];
}
