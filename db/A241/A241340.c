/*
 * A241340  Number of partitions p of n such that floor(mean(p)) and
 *          ceiling(mean(p)) are parts of p.
 * https://oeis.org/A241340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} f(k,n) where f(k,n) = (n%k==0) ?
 *            p_{k-1}(n-n/k) : (k>=2 ? p_{k-2}(n-2*⌊n/k⌋-1) : 0) p_t(s) is the
 *            number of partitions of s into exactly t parts. The function
 *            computes p_t(s) by dynamic programming, storing only three rows
 *            of the recurrence: p_t(s) = p_t(s-t) + p_{t-1}(s-1). Com...
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Allocate three rows of size n+1 each. */
    seq_t *row[3];
    row[0] = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    row[1] = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    row[2] = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));

    if (!row[0] || !row[1] || !row[2]) {
        /* In case of allocation failure, free any allocated memory
           and return 0. This should never happen for normal inputs. */
        free(row[0]); free(row[1]); free(row[2]);
        return (seq_t)0;
    }

    /* row[0] corresponds to t = 0: only the empty partition. */
    row[0][0] = (seq_t)1;          /* p_0(0) = 1 */
    /* All other entries of row[0] are already zero due to calloc. */

    seq_t result = (seq_t)0;
    int idx_cur = 0;                /* index of the current row (t = k-1) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Current row corresponds to t = k-1 */
        seq_t *cur_row   = row[idx_cur];
        seq_t *next_row  = row[(idx_cur + 1) % 3];
        seq_t *prev2_row = row[(idx_cur + 2) % 3];   /* t = k-2 */

        /* Compute contribution for this k */
        idx_t m = n / k;
        if (n % k == 0) {
            /* Mean is integer, need part m to appear */
            idx_t idx = n - m;          /* = n - n/k */
            if (idx <= n) {
                result += cur_row[(size_t)idx];
            }
        } else if (k >= 2) {
            /* Mean is not integer, need both m and m+1 to appear */
            /* Only compute if n >= 2*m+1 */
            if (n >= 2 * m + 1) {
                idx_t idx = n - 2 * m - 1;   /* n - 2*floor(n/k) - 1 */
                if (idx <= n) {
                    result += prev2_row[(size_t)idx];
                }
            }
        }

        /* Build the next row (t = k) using the recurrence:
           p_t(s) = p_t(s-t) + p_{t-1}(s-1)   with t = k.  */
        for (idx_t s = 0; s <= n; ++s) {
            seq_t part1 = (s >= k) ? next_row[(size_t)(s - k)] : (seq_t)0;
            seq_t part2 = (s >= 1) ? cur_row[(size_t)(s - 1)] : (seq_t)0;
            next_row[(size_t)s] = part1 + part2;
        }

        /* Advance to the next k */
        idx_cur = (idx_cur + 1) % 3;
    }

    /* Free the allocated rows */
    free(row[0]); free(row[1]); free(row[2]);

    return result;
}
