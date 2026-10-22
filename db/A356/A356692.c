/*
 * A356692  Pascal-like triangle, where each entry is the sum of the four
 *          entries above it starting with 1 at the top.
 * https://oeis.org/A356692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = max { m >= 0 | m(m+1)/2 <= n } (row
 *            index) * k = n - r(r+1)/2 (column index) * T(0,0) = 1, T(m,k) =
 *            0 if k<0 or k>m * T(m,k) = T(m-1,k-2) + T(m-1,k-1) + T(m-1,k) +
 *            T(m-1,k+1) * This yields the flattened row-major sequence of the
 *            triangle
 * Range:     u8 n <= 28, u16 n <= 67, u32 n <= 196, u64 n <= 674,
 *            u128 n <= 2368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
  #include <cstddef>
#endif

/* -------------------------------------------------------------- */
/* Helper: allocate an array of seq_t of given length                */
static void *alloc_seq_array(size_t len) {
#if defined(__cplusplus)
    return new seq_t[len];
#else
    return (void *)malloc(len * sizeof(seq_t));
#endif
}

/* Helper: free an array returned by alloc_seq_array                */
static void free_seq_array(void *p) {
#if defined(__cplusplus)
    delete[] (seq_t *)p;
#else
    free(p);
#endif
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;   /* column index in row r */

    if (r == 0) {
        return (seq_t)1;   /* T(0,0) = 1 */
    }

    /* Build rows up to r, keeping only the previous row            */
    /* Note: maximum length needed is small (~150 for n<=10000)      */
    size_t prev_len = 1;
    seq_t *prev = (seq_t *)alloc_seq_array(prev_len);
    prev[0] = (seq_t)1;    /* row 0 */

    for (idx_t row = 1; row <= r; ++row) {
        size_t curr_len = (size_t)(row + 1);
        seq_t *curr = (seq_t *)alloc_seq_array(curr_len);

        for (idx_t j = 0; j < row + 1; ++j) {
            seq_t a = 0, b = 0, c = 0, d = 0;
            if (j >= 2)          a = prev[j - 2];
            if (j >= 1)          b = prev[j - 1];
            if (j < prev_len)    c = prev[j];
            if (j + 1 < prev_len) d = prev[j + 1];
            curr[j] = a + b + c + d;
        }

        /* Prepare for next iteration */
        free_seq_array(prev);
        prev = curr;
        prev_len = curr_len;
    }

    /* prev now holds row 'r' */
    seq_t result = prev[(size_t)k];
    free_seq_array(prev);
    return result;
}
