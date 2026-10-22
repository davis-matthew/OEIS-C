/*
 * A400600  Largest m such that one defective in an m-item set and one in a
 *          disjoint 5-item set can always be identified in at most n adaptive
 *          tests, each returning the number of defectives in a selected
 *          subset (0 if no positive m is feasible).
 * https://oeis.org/A400600
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = f(n) + offset where f(n) = 2^n - n*(n-3)/2 - 5 offset = 0
 *            for 2 ≤ n ≤ 8 = 1 for 9 ≤ n ≤ 10 = 2 for n ≥ 11 a(n) is defined
 *            for n ≥ 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 2, behavior is undefined; return 0 by convention. */
    if (n < 2)
        return (seq_t)0;

    /* Compute f(n) = 2^n - n*(n-3)/2 - 5.
       Handle the term n*(n-3)/2, which can be negative for n = 2. */
    seq_t pow2n   = pow2(n);                /* 2^n as seq_t */
    int64_t n_val = (int64_t)n;             /* signed value for safe arithmetic */
    int64_t sub_val = n_val * (n_val - 3) / 2;/* n*(n-3)/2 */

    seq_t result;
    if (sub_val >= 0) {
        /* Standard case: subtract the positive sub_val. */
        result = pow2n - (seq_t)sub_val - (seq_t)5;
    } else {
        /* sub_val is negative; subtracting a negative is adding its absolute value. */
        result = pow2n + (seq_t)(-sub_val) - (seq_t)5;
    }

    /* Add the offset depending on the range of n. */
    if (n >= 9 && n <= 10)
        result += (seq_t)1;
    else if (n >= 11)
        result += (seq_t)2;

    return result;
}
