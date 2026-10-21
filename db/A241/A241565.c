/*
 * A241565  Number of 3-element subsets of {1,...,n} whose sum has more than 4
 *          divisors.
 * https://oeis.org/A241565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   divisor_counts_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = number of 3‑element subsets of {1,…,n} whose sum has more than 4 divisors. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_counts_upto.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) {
        /* For n = 1 or 2 the answer is 0 (from the OEIS offset) */
        return 0;
    }

    /* 1. Compute divisor counts up to 3*n */
    idx_t maxSum = 3 * n;
    idx_t *divcnt = (idx_t *)calloc((size_t)maxSum + 1, sizeof(idx_t));
    if (!divcnt) exit(1);
    divisor_counts_upto(maxSum, divcnt);

    /* 2. Build difference array for counts of unordered distinct triples per sum */
    int64_t *diff = (int64_t *)calloc((size_t)maxSum + 3, sizeof(int64_t));
    if (!diff) exit(1);

    for (idx_t a_val = 1; a_val <= n - 2; ++a_val) {
        for (idx_t b_val = a_val + 1; b_val <= n - 1; ++b_val) {
            /* For fixed a, b, c runs from b+1 to n.
               The corresponding sums range from a + 2*b + 1  to  a + b + n.    */
            idx_t start = a_val + 2 * b_val + 1;
            if (start > maxSum) continue;
            idx_t end = a_val + b_val + n;
            if (end > maxSum) end = maxSum;
            if (start > end) continue;

            ++diff[start];
            --diff[end + 1];
        }
    }

    /* 3. Prefix sum to obtain the actual counts per sum */
    seq_t result = 0;
    int64_t acc = 0;
    for (idx_t s = 3; s <= maxSum; ++s) {
        acc += diff[s];
        if (divcnt[s] > 4) {
            result += (seq_t)acc;
        }
    }

    /* 4. Clean up */
    free(divcnt);
    free(diff);

    return result;
}
