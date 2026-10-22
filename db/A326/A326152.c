/*
 * A326152  Number of integer partitions of n whose product of parts is 2 * n.
 * https://oeis.org/A326152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of 2*n into integers
 *            >1 * whose sum does not exceed n. Each such factorization
 *            corresponds to a * partition of n with product 2*n by inserting
 *            1's as needed. The algorithm * enumerates all factor chains in
 *            non‑decreasing order, pruning when the sum * woul...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Recursive helper: count ways to factor 'val' into multiplicative parts
 * ≥ min_factor such that the sum of chosen parts does not exceed
 * 'remaining_sum'.  It returns the number as seq_t. */
static seq_t dfs(idx_t val, idx_t min_factor, idx_t remaining_sum)
{
    /* If even the smallest allowed factor is too large, no continuation. */
    if (remaining_sum < min_factor)
        return 0;
    /* All factors have been chosen.  Valid partition found. */
    if (val == 1)
        return 1;

    seq_t total = 0;
    /* Try all divisors d of val, starting from min_factor up to sqrt(val). */
    for (idx_t d = min_factor; d <= val / d; ++d) {
        if (d > remaining_sum)          /* further divisors will only be bigger */
            break;
        if (val % d == 0)
            total += dfs(val / d, d, remaining_sum - d);
    }
    /* Special case: take val itself as the next factor. */
    if (val >= min_factor && remaining_sum >= val)
        total += 1;      /* val becomes 1 afterwards, so one valid factorization */

    return total;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* empty partition cannot have product 0 */
    idx_t k = n * 2;
    return dfs(k, 2, n);
}
