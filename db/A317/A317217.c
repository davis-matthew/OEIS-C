/*
 * A317217  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 5, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A317217: Number of n×3 0..1 arrays with forbidden king‑move adjacencies.
 * Recurrence (n>10):
 *   a(n) = 4·a(n-1)+18·a(n-2)+12·a(n-3)
 *          -68·a(n-4)-154·a(n-5)-120·a(n-6)
 *          -66·a(n-7)-43·a(n-8)-12·a(n-9)
 * Initial values (1‑based):
 *   a(1)=1, a(2)=25, a(3)=133, a(4)=929, a(5)=6134,
 *   a(6)=40873, a(7)=272012, a(8)=1810626, a(9)=12051365,
 *   a(10)=80215518.
 * The function memoises results, so calls in any order are O(n) total
 * with no external side effects.                                           */

#include <stdlib.h>

/* Memoised array (1‑based).  memo_sz holds the allocated size.
   computed is the highest index that currently has a value. */
static seq_t *memo = NULL;
static size_t memo_sz = 0;          /* current allocation size */
static idx_t computed = 0;          /* highest computed index (0 if none) */

static const seq_t base_vals[11] = {
    (seq_t)0,   /* unused, index 0 */
    (seq_t)1,   (seq_t)25,  (seq_t)133,  (seq_t)929,
    (seq_t)6134,(seq_t)40873,(seq_t)272012,(seq_t)1810626,
    (seq_t)12051365,(seq_t)80215518
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 safely */

    /* Ensure allocation for index n (1‑based).  new_size = n+1 */
    size_t new_size = (size_t)n + 1;
    if (memo_sz < new_size) {
        void *tmp = realloc(memo, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        memo = (seq_t *)tmp;
        /* zero‑fill newly allocated part */
        for (size_t i = memo_sz; i < new_size; ++i) memo[i] = (seq_t)0;
        memo_sz = new_size;
    }

    /* Initialise base values if not already done */
    if (computed < 10) {
        size_t limit = (computed < 10 ? 10 : (size_t)computed);
        for (size_t i = (size_t)(computed + 1); i <= limit && i < memo_sz; ++i) {
            memo[i] = base_vals[i];
            computed = (idx_t)i;
        }
    }

    /* If n already computed, just return it */
    if (n <= computed) return memo[(size_t)n];

    /* Compute missing terms up to n using the recurrence. */
    size_t start = (size_t)(computed + 1);
    if (start < 11) start = 11;            /* first index computed by recurrence */
    for (size_t i = start; i <= (size_t)n; ++i) {
        memo[i] =   (seq_t)4  * memo[i-1]
                  + (seq_t)18 * memo[i-2]
                  + (seq_t)12 * memo[i-3]
                  - (seq_t)68 * memo[i-4]
                  - (seq_t)154* memo[i-5]
                  - (seq_t)120* memo[i-6]
                  - (seq_t)66 * memo[i-7]
                  - (seq_t)43 * memo[i-8]
                  - (seq_t)12 * memo[i-9];
        computed = (idx_t)i;
    }

    return memo[(size_t)n];
}
