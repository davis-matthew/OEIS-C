/*
 * A348014  Triangle, read by rows, with row n forming the coefficients in
 *          Product_{k=0..n} (1 + k^k*x).
 * https://oeis.org/A348014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 23, u64 n <= 39, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangular array defined by:
 * T(0,0)=1,  T(n,k)=T(n-1,k)+n^n*T(n-1,k-1).
 * a(n) is obtained by flattening the rows of this triangle.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Determine row and column of flat index n */
    idx_t r = (idx_t)((sqrtl((long double)(8 * n + 1)) - 1) / 2);
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t c = n - r * (r + 1) / 2;

    /* Allocate first row */
    seq_t *prev = (seq_t*)calloc(1, sizeof(seq_t));
    prev[0] = (seq_t)1;     /* T(0,0) */

    for (idx_t i = 1; i <= r; ++i) {
        seq_t *curr = (seq_t*)calloc(i + 1, sizeof(seq_t));
        curr[0] = (seq_t)1; /* first element is always 1 */

        /* Compute i^i using exponentiation by squaring */
        seq_t pow_val = (seq_t)1;
        seq_t base = (seq_t)i;
        idx_t e = i;
        while (e) {
            if (e & 1) pow_val *= base;
            base *= base;
            e >>= 1;
        }

        for (idx_t j = 1; j <= i; ++j) {
            seq_t prev_j = (j < i ? prev[j] : (seq_t)0);
            seq_t prev_jm1 = prev[j - 1];
            curr[j] = prev_j + pow_val * prev_jm1;
        }

        free(prev);
        prev = curr;
    }

    seq_t result = prev[c];
    free(prev);
    return result;
}
