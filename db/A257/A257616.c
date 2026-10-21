/*
 * A257616  Triangle read by rows: T(n,k) = t(n-k, k); t(n,m) = f(m)*t(n-1,m)
 *          + f(n)*t(n,m-1), where f(x) = 6*x + 2.
 * https://oeis.org/A257616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where the triangle T is read row‑by‑row. The
 *            triangle satisfies T(0,0) = 1, T(n,0) = 2 * T(n‑1,0), T(n,k) =
 *            (6*k+2) * T(n-1,k) + (6*(n-k)+2) * T(n-1,k-1) for k>0
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 38, u64 n <= 109, u128 n <= 285,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Find r and k such that n = r*(r+1)/2 + k,  0 <= k <= r.
       For n up to tens of thousands a simple loop is fast enough. */
    idx_t r = 0;
    while ((unsigned __int128)r * (r + 1) / 2 <= n)
        ++r;
    --r;                                      /* now r*(r+1)/2 <= n */
    idx_t row_start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - row_start;                   /* column index     */

    /* Static cache: values stored row‑major in a 1‑D array. */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* number of elements allocated   */
    static idx_t computed_rows = 0; /* rows 0 .. computed_rows-1 are known   */

    /* Extend cache if needed and compute missing rows. */
    if (r >= computed_rows) {
        /* How many entries are required to store rows 0..r? */
        unsigned __int128 needed128 = (unsigned __int128)(r + 1) * (r + 2) / 2;
        size_t needed = (size_t)needed128;

        if (needed > cache_size) {
            /* Allocate more space.  The explicit cast works for both C and C++. */
            seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!tmp)          /* allocation failure – return 0 */
                return (seq_t)0;
            cache = tmp;
            cache_size = needed;
        }

        /* Compute rows from computed_rows up to r inclusive. */
        for (idx_t row = computed_rows; row <= r; ++row) {
            idx_t start = (idx_t)((unsigned __int128)row * (row + 1) / 2);
            /* prev_start is defined only for row>0 */
            idx_t prev_start = (row > 0
                                ? (idx_t)((unsigned __int128)(row - 1) * row / 2)
                                : 0);

            /* Row 0 */
            if (row == 0) {
                cache[start] = (seq_t)1;
                continue;
            }

            /* Row > 0 */
            for (idx_t col = 0; col <= row; ++col) {
                seq_t t_prev_col   = (col <= row - 1)
                                     ? cache[prev_start + col]
                                     : (seq_t)0;
                seq_t t_prev_colm1 = (col >= 1)
                                     ? cache[prev_start + col - 1]
                                     : (seq_t)0;

                seq_t coeff1 = (seq_t)(6 * col + 2);
                seq_t coeff2 = (seq_t)(6 * (row - col) + 2);

                cache[start + col] = coeff1 * t_prev_col + coeff2 * t_prev_colm1;
            }
        }
        computed_rows = r + 1;
    }

    /* Return the needed element. */
    return cache[row_start + k];
}
