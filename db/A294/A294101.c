/*
 * A294101  Number of partitions of n into two parts such that one is
 *          squarefree and the other is nonsquarefree.
 * https://oeis.org/A294101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) - Sum_{i=1..floor(n/2)} [mu(i)^2 = mu(n-i)^2]
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static cache of square‑free information */
static uint8_t *mu2_sqfree = NULL;
static idx_t mu2_max = 0;

/* Ensure mu^2 (=1 if square‑free, 0 otherwise) is known up to |limit| */
static void ensure_squarefree_upto(idx_t limit)
{
    if (limit <= mu2_max)
        return;

    /* Allocate a fresh array and initialise to “square‑free” */
    uint8_t *new_arr = (uint8_t *)malloc((size_t)(limit + 1));
    if (!new_arr)
        abort();
    for (idx_t i = 0; i <= limit; ++i)
        new_arr[i] = 1;

    /* Mark all numbers that have a squared prime factor */
    for (idx_t p = 2; p <= limit / p; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= limit; j += sq)
            new_arr[j] = 0;
    }

    /* Replace the old cache */
    free(mu2_sqfree);
    mu2_sqfree = new_arr;
    mu2_max = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1, but guard against n=0 */
        return 0;

    ensure_squarefree_upto(n);

    idx_t half = n >> 1;           /* floor(n/2) */
    idx_t same = 0;                /* count of i with same square‑free status as n-i */

    for (idx_t i = 1; i <= half; ++i) {
        if (mu2_sqfree[i] == mu2_sqfree[n - i])
            ++same;
    }

    return (seq_t)(half - same);
}
