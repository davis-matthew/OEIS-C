/*
 * A374807  a(n) is the smallest number which can be represented as the sum of
 *          4 distinct nonzero triangular numbers in exactly n ways, or -1 if
 *          no such number exists.
 * https://oeis.org/A374807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   compute_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A374807:  a(n) = smallest number which can be represented as
   the sum of 4 distinct non‑zero triangular numbers in exactly n ways,
   or -1 if no such number exists. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_counts.h"

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

/* maximum sum that the routine retries up to before giving up (can be tuned) */
#define MAX_SUM_LIMIT 5000000

/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*  Global state for memoization */
static idx_t g_maxRequest = 0;
static seq_t *g_minSum = NULL;   /* array of size g_maxRequest+1, 1-based */

/*  Ensure that minimal sums up to n are known */
static void ensure_computed(idx_t n)
{
    if (n <= g_maxRequest) return;                     /* already ready */

    /* we will grow the maximum request to at least n */
    idx_t target = n;
    seq_t *newMinSum = NULL;

    /* estimate initial sum limit */
    idx_t sum_limit = target * 500;                     /* heuristic */
    if (sum_limit < 1000) sum_limit = 1000;
    if (sum_limit > MAX_SUM_LIMIT) sum_limit = MAX_SUM_LIMIT;

    /* repeat until all counts are found or sum_limit reached bound */
    while (1) {
        /* allocate temporary result array */
        newMinSum = (seq_t*)calloc((size_t)target + 1, sizeof(seq_t));
        if (!newMinSum) return;                       /* allocation failure */

        compute_counts(sum_limit, target, newMinSum);

        /* check if all counts have a value */
        int all_found = 1;
        for (size_t i = 1; i <= target; ++i) {
            if (newMinSum[i] == (seq_t)0) {
                all_found = 0;
                break;
            }
        }

        if (all_found) {
            /* accept result */
            free(g_minSum);
            g_minSum = newMinSum;
            g_maxRequest = target;
            return;
        }

        /* not all found: try a larger sum limit */
        free(newMinSum);
        newMinSum = NULL;

        /* increase sum_limit exponentially but not beyond bound */
        sum_limit *= 2;
        if (sum_limit > MAX_SUM_LIMIT) {
            sum_limit = MAX_SUM_LIMIT;
        }
        if (sum_limit == MAX_SUM_LIMIT) {
            /* cannot increase further; accept partial data */
            free(g_minSum);
            g_minSum = newMinSum;
            g_maxRequest = target;
            return;
        }
    }
}

/*----------------------------------------------------------------------*/
/*  Public function required by the interface */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)-1;                         /* undefined for n<1 */

    ensure_computed(n);
    seq_t res = g_minSum ? g_minSum[(size_t)n] : (seq_t)0;
    return (res == (seq_t)0) ? (seq_t)-1 : res;
}
