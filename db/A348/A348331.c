/*
 * A348331  Lexicographically earliest sequence of positive integers such that
 *          for any n > 0, the sum of the indices k < n such that a(k) = a(n)
 *          is less than or equal to n.
 * https://oeis.org/A348331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A348331: Lexicographically earliest sequence of positive integers such that 
   for any n > 0, the sum of the indices k < n with a(k) = a(n) is <= n. 
   The sequence is built greedily: at each position n choose the smallest positive 
   integer v for which the current cumulative sum of indices of v does not exceed n. 
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq = NULL;                /* sequence values, 1‑based index */
static idx_t *sum = NULL;                /* cumulative index sums per value, 1‑based */
static size_t seq_cap = 0, sum_cap = 0;   /* allocated capacities */
static idx_t max_n_computed = 0;         /* largest n already computed */

/* Ensure seq array can hold indices up to n (inclusive) */
static void ensure_seq_cap(idx_t n)
{
    while (seq_cap <= (size_t)n) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap << 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Ensure sum array can hold value indices up to v (inclusive) */
static void ensure_sum_cap(idx_t v)
{
    while (sum_cap <= (size_t)v) {
        size_t new_cap = sum_cap == 0 ? 16 : sum_cap << 1;
        if (new_cap <= (size_t)v) new_cap = v + 1;
        sum = (idx_t *)realloc(sum, new_cap * sizeof(idx_t));
        /* initialize new elements to zero */
        for (size_t i = sum_cap; i < new_cap; ++i) sum[i] = 0;
        sum_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, but return 0 */

    if (n > max_n_computed) {
        ensure_seq_cap(n);
        for (idx_t i = max_n_computed + 1; i <= n; ++i) {
            for (idx_t v = 1; ; ++v) {
                ensure_sum_cap(v);
                if (sum[v] <= i) {
                    seq[i] = (seq_t)v;
                    sum[v] += i;
                    break;
                }
            }
        }
        max_n_computed = n;
    }
    return seq[n];
}
