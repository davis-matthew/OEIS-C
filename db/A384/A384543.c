/*
 * A384543  Number of distinct values from the bitwise operation i XOR j for
 *          all integers i and j in the range [1, n].
 * https://oeis.org/A384543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384543: Number of distinct values from the bitwise
   operation i XOR j for all integers i and j in [1, n].  The
   algorithm uses a Walsh–Hadamard transform to compute the
   frequency of each XOR value in O(N log N) time, where N is the
   next power of two greater than 2*2^⌊log₂n⌋.  For n up to about
   10 000 this runs in well under 2 seconds.  The result is the
   number of v such that ∃ i,j∈[1,n] with i^j = v.  */
#include <stdlib.h>

static inline idx_t floor_log2_idx(idx_t n)
{
    idx_t m = 0;
    while (n >>= 1) ++m;
    return m;
}

seq_t a(idx_t n)
{
    /* special case n == 0 is outside the OEIS offset; return 0 */
    if (n == 0) return 0;

    /* compute high = 2^⌊log₂n⌋ */
    idx_t m   = floor_log2_idx(n);
    idx_t high = 1ULL << m;            /* high >= 1 */
    idx_t max_val = high << 1;         /* 2*high */

    /* N is next power of two greater than max_val */
    size_t N = 1;
    while (N <= max_val) N <<= 1;

    /* prepare array for FWHT */
    int64_t *a_arr = (int64_t *)malloc(N * sizeof(int64_t));
    if (!a_arr) return 0;              /* out of memory */

    for (size_t i = 0; i < N; ++i)
        a_arr[i] = (i >= 1 && i <= n) ? 1 : 0;

    /* forward Walsh–Hadamard transform (XOR) */
    for (size_t len = 1; len < N; len <<= 1) {
        for (size_t i = 0; i < N; i += len << 1) {
            for (size_t j = 0; j < len; ++j) {
                int64_t u = a_arr[i + j];
                int64_t v = a_arr[i + j + len];
                a_arr[i + j]         = u + v;
                a_arr[i + j + len]   = u - v;
            }
        }
    }

    /* pointwise square: counts of ordered pairs */
    for (size_t i = 0; i < N; ++i)
        a_arr[i] = a_arr[i] * a_arr[i];

    /* inverse transform */
    for (size_t len = 1; len < N; len <<= 1) {
        for (size_t i = 0; i < N; i += len << 1) {
            for (size_t j = 0; j < len; ++j) {
                int64_t u = a_arr[i + j];
                int64_t v = a_arr[i + j + len];
                a_arr[i + j]         = u + v;
                a_arr[i + j + len]   = u - v;
            }
        }
    }

    /* divide by N to obtain actual counts */
    size_t distinct = 0;
    for (size_t i = 0; i < N; ++i) {
        int64_t val = a_arr[i] / (int64_t)N;
        if (val > 0) ++distinct;
    }

    free(a_arr);
    return (seq_t)distinct;
}
