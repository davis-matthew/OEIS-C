/*
 * A339365  Number of partitions of n into an odd number of squares.
 * https://oeis.org/A339365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an odd number of perfect
 *            squares. Computation: dynamic programming over the set of square
 *            numbers [1^2, 2^2, …]. For each square s² we update two arrays:
 *            dp_even[sum] – partitions of sum into an even number of squares
 *            dp_odd [sum] – partitions of sum into an...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Convert index to a native loop size. */
    idx_t N = n;
    size_t len = (size_t)(N + 1);

    /* Allocate arrays for the even/odd counts. */
#ifdef __cplusplus
    std::vector<seq_t> dp_even(len), dp_odd(len);
#else
    seq_t *dp_even = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *dp_odd  = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp_even || !dp_odd)  /* allocation failure – return 0 */
        return (seq_t)0;
#endif

    /* Initialize counts. */
#ifdef __cplusplus
    dp_even[0] = (seq_t)1;
    dp_odd[0]  = (seq_t)0;
    for (size_t i = 1; i < len; ++i) {
        dp_even[i] = (seq_t)0;
        dp_odd[i]  = (seq_t)0;
    }
#else
    dp_even[0] = (seq_t)1;
    dp_odd[0]  = (seq_t)0;
    for (size_t i = 1; i < len; ++i) {
        dp_even[i] = (seq_t)0;
        dp_odd[i]  = (seq_t)0;
    }
#endif

    /* Dynamic programming over all square values. */
    for (idx_t s = 1; (idx_t)(s * s) <= N; ++s) {
        idx_t square = s * s;
        for (idx_t sum = square; sum <= N; ++sum) {
            dp_even[sum] += dp_odd[sum - square];
            dp_odd [sum] += dp_even[sum - square];
        }
    }

    /* Result is the number of partitions with an odd number of squares. */
    seq_t result = dp_odd[N];

#ifdef __cplusplus
    return result;
#else
    free(dp_even);
    free(dp_odd);
    return result;
#endif
}
