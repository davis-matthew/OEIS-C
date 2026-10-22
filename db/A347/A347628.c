/*
 * A347628  Number of partitions of n into at most 6 triangular numbers.
 * https://oeis.org/A347628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into at most 6 triangular numbers *
 *            (triangular numbers: 1, 3, 6, 10, 15, …). * a(0) = 1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Cache the already computed values */
    static seq_t *answers = NULL;        /* answers[i] = a(i) for 0 <= i <= cached_max */
    static idx_t cached_max = 0;         /* highest index that has been cached */

    /* If we already have the requested value, just return it */
    if (n <= cached_max && answers)
        return answers[n];

    /* We need to recompute up to new_max */
    idx_t new_max = n;

    /* ----------------- DP tables ----------------------------------- */
    /* dp[k][i] = number of partitions of i using exactly k triangular numbers
     * k = 0..6, i = 0..new_max
     */
    seq_t *dp[7];
    for (int k = 0; k <= 6; ++k) {
        dp[k] = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));
        /* initialise all entries to zero */
        memset(dp[k], 0, (new_max + 1) * sizeof(seq_t));
    }

    /* Base case: 0 can be partitioned with 0 triangular numbers in exactly
     * one way, the empty partition. */
    dp[0][0] = 1;

    /* Enumerate all triangular numbers up to new_max */
    for (idx_t r = 1; ; ++r) {
        idx_t t = (r * (r + 1)) / 2;        /* triangular number in idx_t */
        if (t > new_max) break;

        /* For each triangular number, update the dp tables in the classic
         * coin-change manner, but keeping track of the number of parts used. */
        for (idx_t i = t; i <= new_max; ++i) {
            for (int k = 1; k <= 6; ++k) {
                dp[k][i] += dp[k - 1][i - t];
            }
        }
    }

    /* ----------------- Build the answer cache --------------------- */
    /* Reallocate answers array if necessary */
    if (answers) {
        free(answers);
    }
    answers = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));

    for (idx_t i = 0; i <= new_max; ++i) {
        seq_t total = 0;
        for (int k = 0; k <= 6; ++k) {
            total += dp[k][i];
        }
        answers[i] = total;
    }

    /* Update cached range */
    cached_max = new_max;

    /* Free temporary DP arrays */
    for (int k = 0; k <= 6; ++k) {
        free(dp[k]);
    }

    return answers[n];
}
