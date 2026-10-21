/*
 * A296371  Number of integer partitions of n using Jacobsthal numbers.
 * https://oeis.org/A296371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 212, u32 n <= 1513, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of integer partitions of n using Jacobsthal numbers
   (unlimited use of each part, 1 used only once).
   The function implements the classic coin‑change DP algorithm
   with the Jacobsthal denominations J(1)=1, J(3)=3, J(4)=5,
   J(5)=11, J(6)=21, …  */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate DP table */
    size_t cap = (size_t)(n + 1);
    seq_t *dp = alloc_seq_array(cap);
    if (!dp)
        return 0;                /* allocation failure */

    /* initialise dp[0] = 1, rest 0 */
    dp[0] = 1;
    for (size_t i = 1; i < cap; ++i)
        dp[i] = 0;

    /* Process the first positive Jacobsthal number 1 (once) */
    if (n >= 1) {
        for (idx_t s = 1; s <= n; ++s)
            dp[(size_t)s] += dp[(size_t)(s - 1)];
    }

    /* Generate remaining Jacobsthal numbers */
    idx_t prev = 0;   /* J(0) */
    idx_t cur  = 1;   /* J(1) */
    while (1) {
        idx_t next = cur + 2 * prev;   /* J(k) for k >= 2 */
        if (next > n)
            break;

        /* skip duplicate 1 (J(2)) */
        if (next != 1) {
            for (idx_t s = next; s <= n; ++s)
                dp[(size_t)s] += dp[(size_t)(s - next)];
        }

        /* advance recurrence */
        prev = cur;
        cur  = next;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
