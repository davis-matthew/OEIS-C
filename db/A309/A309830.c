/*
 * A309830  Sum of the odd parts appearing among the smallest parts of the
 *          partitions of n into 5 parts.
 * https://oeis.org/A309830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{l odd, 1 ≤ l ≤ ⌊n/5⌋} l * p4(n-5l) where p4(m) =
 *            number of partitions of m into at most 4 parts. This is
 *            equivalent to counting partitions of n into 5 parts with
 *            smallest part l, multiplied by l if l is odd
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for p4 values up to the largest n previously seen */
    static seq_t *p4 = NULL;
    static idx_t max_precomputed = 0;

    /* ensure p4 is computed up to n */
    if (n > max_precomputed) {
        /* allocate new array of size n+1 */
#ifdef __cplusplus
        seq_t *new_arr = new seq_t[n + 1]();   /* value-initialised to zero */
#else
        seq_t *new_arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= n; ++i) new_arr[i] = 0;
#endif

        /* compute partitions into at most 4 parts (via parts ≤4) */
        new_arr[0] = 1;
        for (idx_t k = 1; k <= 4; ++k) {
            for (idx_t m = k; m <= n; ++m) {
                new_arr[m] += new_arr[m - k];
            }
        }

        /* free old cache if it existed */
        if (p4 != NULL) {
#ifdef __cplusplus
            delete[] p4;
#else
            free(p4);
#endif
        }
        p4 = new_arr;
        max_precomputed = n;
    }

    /* compute the sum over odd smallest parts */
    seq_t sum = 0;
    for (idx_t l = 1; l <= n / 5; l += 2) {
        seq_t prod = p4[n - 5 * l];
        sum += (seq_t)l * prod;
    }
    return sum;
}
