/*
 * A257610  Triangle read by rows: T(n,k) = t(n-k, k); t(n,m) = f(m)*t(n-1,m)
 *          + f(n)*t(n,m-1), where f(x) = 3*x + 2.
 * https://oeis.org/A257610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle T(n,k) with T(0,0)=1 and * T(n,k) = (3*k+2)*T(n-1,k) +
 *            (3*(n-k)+2)*T(n-1,k-1) for 0≤k≤n * (T(n,k)=0 when k>n). Sequence
 *            a(n) is obtained by reading the * triangle row by row:
 *            a(offset(n)+k)=T(n,k) where * offset(n)=n*(n+1)/2 is the start
 *            index of row n
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 48, u64 n <= 140,
 *            u128 n <= 361, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *triangle = NULL;    /* Stores triangle entries row by row */
static idx_t last_computed_row = 0;   /* Highest row fully computed */

/* Compute a(n) */
seq_t a(idx_t n) {
    /* Find row r such that offset(r) ≤ n < offset(r+1) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t k = n - r * (r + 1) / 2;

    /* Ensure triangle up to row r is computed */
    if (triangle == NULL) {
        /* Allocate first row */
        triangle = (seq_t *)realloc(NULL, sizeof(seq_t));
        triangle[0] = (seq_t)1;      /* T(0,0) = 1 */
        last_computed_row = 0;
    }

    while (last_computed_row < r) {
        idx_t next = last_computed_row + 1;
        /* Compute offsets */
        size_t prev_offset = (size_t)last_computed_row * (last_computed_row + 1) / 2;
        size_t curr_offset = (size_t)next * (next + 1) / 2;

        /* Grow array to accommodate new row */
        size_t new_total = curr_offset + next + 1;  /* number of elements after adding row next */
        triangle = (seq_t *)realloc(triangle, new_total * sizeof(seq_t));

        /* Compute row 'next' */
        /* k = 0 case */
        triangle[curr_offset] = (seq_t)2 * triangle[prev_offset];  /* f(0)=2 */

        /* k >= 1 */
        for (idx_t col = 1; col <= next; col++) {
            seq_t val1 = 0;
            if (col <= (idx_t)last_computed_row) {
                seq_t f_k = (seq_t)(3 * (long long)col + 2);
                val1 = f_k * triangle[prev_offset + col];
            }
            seq_t f_nk = (seq_t)(3 * (long long)(next - col) + 2);
            seq_t val2 = f_nk * triangle[prev_offset + col - 1];
            triangle[curr_offset + col] = val1 + val2;
        }

        last_computed_row = next;
    }

    /* Return the requested entry */
    size_t idx = (size_t)r * (r + 1) / 2 + k;
    return triangle[idx];
}
