/*
 * A320680  Number of 5-element subsets of [n] having a prime element sum.
 * https://oeis.org/A320680
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5‑element subsets of {1,…,n} whose element sum
 *            is prime. Compute via dynamic programming on subset size and sum
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 324, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* offset is 5 */

    /* maximum possible sum of 5 distinct numbers from 1..n */
    idx_t max_sum = 5 * n;

    /* allocate arrays for counts of subsets of size 0..5 */
    seq_t *c0 = (seq_t*)malloc((max_sum + 1) * sizeof(seq_t)); /* not used but for convenience */
    seq_t *c1 = (seq_t*)malloc((max_sum + 1) * sizeof(seq_t));
    seq_t *c2 = (seq_t*)malloc((max_sum + 1) * sizeof(seq_t));
    seq_t *c3 = (seq_t*)malloc((max_sum + 1) * sizeof(seq_t));
    seq_t *c4 = (seq_t*)malloc((max_sum + 1) * sizeof(seq_t));
    seq_t *c5 = (seq_t*)malloc((max_sum + 1) * sizeof(seq_t));

    /* initialise all to zero */
    memset(c0, 0, (max_sum + 1) * sizeof(seq_t));
    memset(c1, 0, (max_sum + 1) * sizeof(seq_t));
    memset(c2, 0, (max_sum + 1) * sizeof(seq_t));
    memset(c3, 0, (max_sum + 1) * sizeof(seq_t));
    memset(c4, 0, (max_sum + 1) * sizeof(seq_t));
    memset(c5, 0, (max_sum + 1) * sizeof(seq_t));

    /* dynamic programming:
       For each new element i, update counts from size k-1 to k.
       Use previous counts only. Execute from largest k to smallest to avoid overwriting.  */
    for (idx_t i = 1; i <= n; ++i) {
        /* size 5 from size 4 */
        idx_t prev_max4 = 4 * (i - 1);
        for (idx_t s = 0; s <= prev_max4; ++s) {
            seq_t val = c4[s];
            if (val) c5[s + i] += val;
        }

        /* size 4 from size 3 */
        idx_t prev_max3 = 3 * (i - 1);
        for (idx_t s = 0; s <= prev_max3; ++s) {
            seq_t val = c3[s];
            if (val) c4[s + i] += val;
        }

        /* size 3 from size 2 */
        idx_t prev_max2 = 2 * (i - 1);
        for (idx_t s = 0; s <= prev_max2; ++s) {
            seq_t val = c2[s];
            if (val) c3[s + i] += val;
        }

        /* size 2 from size 1 */
        idx_t prev_max1 = (i - 1);
        for (idx_t s = 0; s <= prev_max1; ++s) {
            seq_t val = c1[s];
            if (val) c2[s + i] += val;
        }

        /* size 1 from empty set */
        c1[i] += 1;
    }

    /* sieve of Eratosthenes up to max_sum */
    bool *is_p = (bool*)malloc((max_sum + 1) * sizeof(bool));
    for (idx_t i = 0; i <= max_sum; ++i) is_p[i] = true;
    if (max_sum >= 0) is_p[0] = false;
    if (max_sum >= 1) is_p[1] = false;
    for (idx_t p = 2; p * p <= max_sum; ++p) {
        if (is_p[p]) {
            for (idx_t q = p * p; q <= max_sum; q += p)
                is_p[q] = false;
        }
    }

    /* sum counts for prime sums */
    seq_t result = 0;
    for (idx_t s = 0; s <= max_sum; ++s) {
        if (is_p[s]) result += c5[s];
    }

    /* free memory */
    free(c0); free(c1); free(c2); free(c3); free(c4); free(c5);
    free(is_p);

    return result;
}
