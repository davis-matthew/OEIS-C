/*
 * A318685  Triangle read by rows: T(0,0) = 1; T(n,k) = 2 T(n-1,k) - 3
 *          T(n-1,k-1) + T(n-1,k-2) for k = 0..2n; T(n,k)=0 for n or k < 0.
 * https://oeis.org/A318685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 52, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A318685 - Triangle read by rows
   T(0,0)=1;  T(n,k)=2*T(n-1,k)-3*T(n-1,k-1)+T(n-1,k-2)  for 0≤k≤2n
   a(n) = T(⌊√n⌋, n-⌊√n⌋²)   (flattened triangle)
   This implementation uses the recurrence directly and works for all
   n≥0, with arbitrary‑precision seq_t. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Floor square‐root of a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1ULL << 32;            /* (2³²)² = 2⁶⁴  */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0u)          /* row 0, column 0 */
        return (seq_t)1;

    idx_t r = isqrt_u64(n);        /* row number */
    idx_t k = n - r * r;           /* column within row r */
    if (k > 2 * r)                 /* should not happen */
        return (seq_t)0;

    /* Length of the largest row that will be needed. */
    size_t len = (size_t)(2 * r + 1);

    /* Two working arrays of the same size. */
    seq_t *prev = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *cur  = (seq_t*)malloc(len * sizeof(seq_t));
    if (!prev || !cur) {
        free(prev); free(cur);
        return (seq_t)0;
    }

    /* Initial row 0. */
    for (size_t i = 0; i < len; ++i) prev[i] = (seq_t)0;
    prev[0] = (seq_t)1;

    size_t prev_len = 1;                 /* number of valid entries in prev */

    for (idx_t level = 1; level <= r; ++level) {
        size_t cur_len = (size_t)(2 * level + 1);

        for (size_t idx = 0; idx < cur_len; ++idx) {
            seq_t a_val = (idx < prev_len) ? prev[idx] : (seq_t)0;
            seq_t b_val = (idx > 0 && idx - 1 < prev_len) ? prev[idx - 1] : (seq_t)0;
            seq_t c_val = (idx > 1 && idx - 2 < prev_len) ? prev[idx - 2] : (seq_t)0;
            cur[idx] = 2 * a_val - 3 * b_val + c_val;
        }
        /* Swap buffers for next iteration. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
        prev_len = cur_len;
    }

    seq_t result = prev[k];
    free(prev);
    free(cur);
    return result;
}
