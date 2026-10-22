/*
 * A394318  Triangle T(n,k), n >= 0, 0 <= k <= n, read by rows, where T(0,0) =
 *          1 and T(n,k) = 2*(k+1)*(2*k+1) * T(n-1,k+1) + (4*(4*k + 1)*n +
 *          8*k^2 - 8*k - 3) * T(n-1,k) + 2*(4*n + 2*k - 5)*(2*n + k - 1) *
 *          T(n-1,k-1), with T(n,k) = 0 if k < 0 or k >= n+1.
 * https://oeis.org/A394318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The triangular array T(n,k) is defined by * T(0,0) = 1 * T(n,k)
 *            = 2*(k+1)*(2*k+1)*T(n-1,k+1) * + (4*(4*k+1)*n + 8*k*k - 8*k -
 *            3)*T(n-1,k) * + 2*(4*n+2*k-5)*(2*n+k-1)*T(n-1,k-1) * with
 *            T(n,k)=0 for k<0 or k>=n+1. * Sequence a(n) is obtained by
 *            reading the triangle row‑by‑row: * a(0)=T(0,0), a(1)=T...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 16, u64 n <= 39, u128 n <= 99,
 *            big any n
 * Helpers:   free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/free_seq_array.h"

/* Helper to allocate a new array of seq_t elements.  */
static seq_t *alloc_seq_array(size_t n) {
    return (seq_t *)malloc(n * sizeof(seq_t));
}

/* Helper to free a seq_t array.  */

/* Dynamic cache of computed rows of the triangle.  */
static seq_t **cached_rows = NULL;
static size_t cached_len = 0;   /* number of rows currently cached (max row index +1) */

/* Ensure that the cache can hold rows up to 'max_row' inclusive.  */
static void ensure_cache(idx_t max_row)
{
    if (cached_len > max_row)
        return;                                 /* already sufficient */
    size_t new_len = max_row + 1;
    if (cached_rows == NULL) {
        cached_rows = (seq_t **)malloc(new_len * sizeof(seq_t *));
    } else {
        cached_rows = (seq_t **)realloc(cached_rows, new_len * sizeof(seq_t *));
    }
    for (size_t i = cached_len; i < new_len; ++i) {
        /* allocate row i with i+1 coefficients */
        seq_t *row = alloc_seq_array(i + 1);
        for (size_t k = 0; k <= i; ++k) {
            seq_t val = 0;
            if (i == 0 && k == 0) {
                val = 1;                       /* base case T(0,0)=1 */
            } else {
                const seq_t prev_k   = (k   <= i-1) ? cached_rows[i-1][k]   : 0;
                const seq_t prev_kp1 = (k+1 <= i-1) ? cached_rows[i-1][k+1] : 0;
                const seq_t prev_km1 = (k   >= 1    ) ? cached_rows[i-1][k-1] : 0;

                /* term1: 2*(k+1)*(2*k+1) * T(n-1,k+1) */
                seq_t coeff1 = (seq_t)2 *
                               ((seq_t)(k + 1)) *
                               ((seq_t)(2 * k + 1));
                seq_t term1 = coeff1 * prev_kp1;

                /* term2: (4*(4*k+1)*n + 8*k*k - 8*k - 3) * T(n-1,k) */
                seq_t coeff2 = (seq_t)4 * ((seq_t)(4 * k + 1)) * ((seq_t)i)
                               + (seq_t)8 * ((seq_t)(k * k))
                               - (seq_t)8 * ((seq_t)k)
                               - (seq_t)3;
                seq_t term2 = coeff2 * prev_k;

                /* term3: 2*(4*n+2*k-5)*(2*n+k-1) * T(n-1,k-1) */
                seq_t coeff3 = (seq_t)2 *
                               ((seq_t)(4 * i + 2 * k - 5)) *
                               ((seq_t)(2 * i + k - 1));
                seq_t term3 = coeff3 * prev_km1;

                val = term1 + term2 + term3;
            }
            row[k] = val;
        }
        cached_rows[i] = row;
    }
    cached_len = new_len;
}

/* Main function: return a(n).  */
seq_t a(idx_t n)
{
    /* Determine row r and column k such that
     *   r*(r+1)/2 <= n < (r+1)(r+2)/2
     *   k = n - r*(r+1)/2
     */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum + r + 1 <= n) {
        ++r;
        sum += r;
    }
    idx_t k = n - sum;

    /* Compute triangle rows up to r if necessary. */
    ensure_cache(r);

    return cached_rows[r][k];
}
