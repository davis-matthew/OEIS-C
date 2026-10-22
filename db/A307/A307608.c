/*
 * A307608  Number of partitions of n^2 into consecutive positive squares.
 * https://oeis.org/A307608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (i,j) | 1 ≤ i ≤ j, Σ_{k=i}^j k^2 = n^2 }
 * Range:     u8 n <= 15, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to write n² as a sum of consecutive positive squares. */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Dynamically grown array of cumulative sums of squares:
   cum[m] = 1² + 2² + … + m²  (cum[0] = 0).  */
static seq_t *cum = NULL;          /* pointer to array of seq_t */
static idx_t  cum_len = 0;         /* number of elements currently valid (cum_len-1 is last) */
static idx_t  cum_cap = 0;         /* allocated capacity */

/* Ensure that cum contains entries up to 'limit' (inclusive).  */
static void ensure_cum(idx_t limit)
{
    if (cum_len > limit)
        return;                    /* already computed */

    /* Grow the allocation if necessary.  */
    if (limit >= cum_cap) {
        idx_t new_cap = cum_cap ? cum_cap * 2 : 16;
        if (new_cap <= limit)
            new_cap = limit + 1;

        seq_t *tmp = seq_realloc(cum, (size_t)cum_cap, (size_t)new_cap);
        if (!tmp) return;          /* out of memory: nothing we can do */
        cum   = tmp;
        cum_cap = new_cap;
    }

    /* Compute missing cumulative sums.  */
    for (idx_t m = cum_len; m <= limit; ++m) {
        seq_t sq = (seq_t)m * (seq_t)m;   /* m² */
        if (m == 0) {
            cum[m] = (seq_t)0;            /* cum[0] = 0 */
        } else {
            cum[m] = cum[m - 1] + sq;     /* cum[m] = cum[m-1] + m² */
        }
    }
    cum_len = limit + 1;   /* next free index */
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    idx_t max_j = n;               /* j never exceeds n because j² ≤ n² */
    ensure_cum(max_j);

    seq_t n_sq = (seq_t)n * (seq_t)n;   /* n² */
    seq_t count = (seq_t)0;             /* result counter */

    /* For each possible start i */
    for (idx_t i = 1; i <= max_j; ++i) {
        seq_t i_sq = (seq_t)i * (seq_t)i;
        if (i_sq > n_sq)
            break;                    /* further i will only increase i² */

        seq_t target = n_sq + cum[i - 1];   /* cum[j] must equal this value */

        /* Binary search for j in [i, max_j] such that cum[j] == target. */
        idx_t low  = i;
        idx_t high = max_j;
        while (low <= high) {
            idx_t mid = low + ((high - low) >> 1);
            seq_t val = cum[mid];

            if (val == target) {
                count = count + (seq_t)1;  /* found a valid block */
                break;
            } else if (val < target) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }

    return count;
}
