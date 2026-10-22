/*
 * A339619  Number of integer partitions of n with no 1's and a part divisible
 *          by all the other parts.
 * https://oeis.org/A339619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{L=2..n} p_div(L, n-L) where p_div(L, m) is the
 *            number of partitions of m into parts that divide L. (All
 *            partitions use parts ≥2 and include at least one part equal to
 *            L.)
 * Range:     u8 n <= 41, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   compute_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_table.h"

/* Assume seq_t and idx_t are defined elsewhere. */
static seq_t *memo = NULL;
static idx_t memo_max = 0;
static bool memo_ok = false;

seq_t a(idx_t n)
{
    if (!memo_ok || n > memo_max) {
        seq_t *newtable = compute_table(n);
        if (memo_ok) {
            free(memo);
        }
        memo = newtable;
        memo_max = n;
        memo_ok = true;
    }
    return memo[n];
}
