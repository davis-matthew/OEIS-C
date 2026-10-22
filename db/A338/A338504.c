/*
 * A338504  Number of integers less than n with the same sum of proper
 *          divisors as n.
 * https://oeis.org/A338504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{j < n : sigma(j) - j = sigma(n) - n}|
 * Range:     u8 n <= 413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"


/* static table holding sigma(j) - j for 0 .. current_cap */
static seq_t *proper_sum = NULL;
static idx_t current_cap = 0;

/* recompute sigma(j) - j for all j ≤ limit */
static void recompute_proper_sum(idx_t limit)
{
    if (proper_sum)
        seq_free_array(proper_sum);

    /* allocate space for indices 0 .. limit inclusive */
    proper_sum = seq_alloc_new_array(limit + 1);

    /* zero initialise the array */
    for (idx_t k = 0; k <= limit; ++k)
        proper_sum[k] = (seq_t)0;

    /* sieve: for each divisor i add i to all multiples j > i */
    for (idx_t i = 1; i <= limit; ++i)
    {
        idx_t j = i << 1;          /* start at 2*i */
        for (; j <= limit; j += i)
        {
            proper_sum[j] += (seq_t)i;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* extend table if necessary */
    if (n > current_cap)
    {
        current_cap = n;
        recompute_proper_sum(current_cap);
    }

    seq_t target = proper_sum[n];
    seq_t count   = 0;

    for (idx_t j = 1; j < n; ++j)
        if (proper_sum[j] == target)
            ++count;

    return count;
}
