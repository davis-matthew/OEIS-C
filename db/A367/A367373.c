/*
 * A367373  Expansion of the e.g.f. (exp(x) / (4 - 3*exp(x)))^(3/4).
 * https://oeis.org/A367373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 3*a(n-1) + 3*sum_{k=1}^{n-1} binomial(n-1,k) *
 *            a(n-k) (n ≥ 1) The implementation follows this recurrence with
 *            no floating point or external libraries other than the provided
 *            arbitrary‑precision helpers for memory allocation. All
 *            arithmetic is performed directly on seq_t values usi...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Compute OEIS sequence A367373 */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate array of seq_t to store values up to n */
    seq_t *arr = seq_alloc_new_array((size_t)(n + 1));
    arr[0] = (seq_t)1;      /* a(0) = 1 */

    /* Compute a(1)..a(n) using the recurrence */
    for (idx_t r = 1; r <= n; ++r) {
        seq_t sum = 0;            /* accumulator for the convolution sum */
        seq_t binom = 1;          /* binomial coefficient C(r-1, j) */
        /* Sum over k = 1 .. r-1 : C(r-1, k) * a(r-k)   */
        for (idx_t k = 1; k < r; ++k) {
            /* Update binom to C(r-1, k) from C(r-1, k-1) */
            binom = binom * (seq_t)(r - k) / (seq_t)k;

            /* term = binom * a(r-k) ;  both operands may be large */
            sum += binom * arr[r - k];
        }
        /* a(r) = 3 * (a(r-1) + sum)   */
        arr[r] = (arr[r - 1] + sum) * (seq_t)3;
    }

    /* Retrieve result, free temporary array, and return */
    seq_t result = arr[n];
    seq_free_array(arr);
    return result;
}
