/*
 * A303941  Triangle read by rows: T(0,0) = 1; T(n,k) = 3*T(n-1,k) -
 *          2*T(n-2,k-1) for k = 0..floor(n/2); T(n,k)=0 for n or k < 0.
 *          Triangle of coefficients of Fermat polynomials.
 * https://oeis.org/A303941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, easy, sign
 * Formula:   a(n) is the n‑th entry of the triangle defined by T(0,0)=1 and
 *            T(n,k)=3*T(n-1,k)-2*T(n-2,k-1) for 0≤k≤⌊n/2⟋ a(n) is obtained by
 *            reading the triangle row by row, left to right. The algorithm
 *            finds the row r and position k corresponding to n, then builds
 *            successive rows up to r using the recurrence
 * Range:     i8 n <= 8, i16 n <= 25, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t is an unsigned integer type (uint64_t) and seq_t is the
   signed integer type used for sequence values, as provided by the
   build environment. */

#include <stdlib.h>
#include <stddef.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n. */
static idx_t floor_sqrt_u64(idx_t n) {
    idx_t x = 0;
    idx_t bit = 1ULL << 62;          /* highest power of four ≤ max ull */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

seq_t a(idx_t n) {
    /* Handle trivial case */
    if (n == 0)
        return (seq_t)1;

    /* Determine the row r and column k of the triangle entry */
    idx_t m = floor_sqrt_u64(n);           /* floor(sqrt(n)) */
    idx_t r, prev_count;
    if (n < m * (m + 1)) {                 /* row is odd: r = 2*m-1 */
        r = 2 * m - 1;
        prev_count = m * m;
    } else {                               /* row is even: r = 2*m     */
        r = 2 * m;
        prev_count = m * (m + 1);
    }
    idx_t k = n - prev_count;              /* 0‑based index within row r */

    /* Dynamic construction of rows up to r */
    seq_t *row_prev2 = NULL;
    seq_t *row_prev1 = NULL;
    size_t len_prev2 = 0;
    size_t len_prev1 = 0;

    for (idx_t i = 0; i <= r; ++i) {
        size_t len = i / 2 + 1;
        seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cur)
            return (seq_t)0;           /* safety: memory allocation failed */

        if (i == 0) {                   /* base case T(0,0)=1 */
            cur[0] = 1;
        } else {
            for (size_t j = 0; j < len; ++j) {
                seq_t t1 = 0, t2 = 0;
                if (i > 0 && j < len_prev1)
                    t1 = 3 * row_prev1[j];
                if (i > 1 && j > 0 && (j - 1) < len_prev2)
                    t2 = 2 * row_prev2[j - 1];
                cur[j] = t1 - t2;
            }
        }

        /* Shift rows for next iteration */
        free(row_prev2);
        row_prev2 = row_prev1;
        len_prev2 = len_prev1;
        row_prev1 = cur;
        len_prev1 = len;
    }

    seq_t result = row_prev1[k];
    free(row_prev1);
    return result;
}
