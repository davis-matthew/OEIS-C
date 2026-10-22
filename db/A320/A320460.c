/*
 * A320460  Alphabetic length of a divide-and-conquer approach to the regular
 *          expression for permutations of n symbols.
 * https://oeis.org/A320460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor(n/2)) * (a(floor(n/2)) + a(ceil(n/2))),
 *            a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* Memoization table for already computed values */
static seq_t *memo = NULL;       /* indices 0..memo_cap-1 are valid */
static idx_t memo_cap = 0;       /* current capacity of the table */

/* Recursive computation with memoization */
seq_t a(idx_t n)
{
    /* automatically defined for n >= 1 */
    if (n == 0)       /* safety guard – undefined case – return 0 */
        return 0;

    /* Allocate or grow the memo table to hold index n */
    if (memo_cap <= n) {
        idx_t new_cap = n + 1;               /* indices 0 .. n inclusive */
        seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!new_mem)                        /* allocation failed */
            abort();
        memo = new_mem;
        /* initialise newly allocated slots to zero */
        for (idx_t i = memo_cap; i < new_cap; ++i)
            memo[i] = 0;
        memo_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (memo[n] != 0)
        return memo[n];

    /* Base case */
    if (n == 1) {
        memo[1] = 1;
        return 1;
    }

    idx_t k  = n / 2;          /* floor(n/2)   */
    idx_t k2 = n - k;          /* ceil(n/2)    */

    seq_t left  = a(k);
    seq_t right = a(k2);

    seq_t bin = binomial(n, k);
    memo[n] = bin * (left + right);

    return memo[n];
}
