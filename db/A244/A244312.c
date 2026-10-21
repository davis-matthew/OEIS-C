/*
 * A244312  Triangle read by rows: T(n,k) is the number of single loop
 *          solutions formed by n proper arches (connecting odd vertices to
 *          even vertices in the range 1 to 2n) above the x axis, k of which
 *          connect an odd vertex to a higher even vertex, with a rainbow of n
 *          arches below the x axis.
 * https://oeis.org/A244312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (k + ⌊(-1)^(n-1)/2⌋) · T(n-1,k) + (n-k -
 *            ⌊(-1)^(n-1)/2⌋) · T(n-1,k-1) with T(1,1)=1, T(n,0)=T(n,n+1)=0.
 *            The flattened sequence a(m) is the sequence of all T(n,k) read
 *            row by row. The implementation builds the required rows on
 *            demand and caches the flattened values in a contiguous array
 *            f...
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    static seq_t *prev_row   = NULL;
    static idx_t   prev_len  = 0;          /* length of previous row (equal to its n) */

    if (n == 0) return 0;                  /* offset is 1; behaviour below 1 is undefined */

    /* Compute additional rows until we have at least n flattened terms */
    while (cache_len < (size_t)n) {
        idx_t    row = prev_len + 1;       /* next row number to compute */
        seq_t   *new_row = (seq_t *)malloc((row + 1) * sizeof(seq_t));
        if (!new_row) return 0;           /* allocation failure – return 0 */

        new_row[0] = 0;                   /* unused, simplifies access */

        if (row == 1) {                   /* base case */
            new_row[1] = (seq_t)1;
        } else {
            long long floor = (row % 2 == 0) ? -1LL : 0LL;   /* floor((-1)^(row-1)/2) */
            for (idx_t k = 1; k <= row; ++k) {
                seq_t left  = (k <= prev_len)    ? prev_row[k]      : 0;
                seq_t right = (k-1 <= prev_len)  ? prev_row[k-1]    : 0;

                seq_t coeff_left  = (seq_t)((long long)k + floor);
                seq_t coeff_right = (seq_t)((long long)(row - k) - floor);

                new_row[k] = coeff_left * left + coeff_right * right;
            }
        }

        /* Ensure the cache has enough capacity to store the new row */
        size_t needed = cache_len + row;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { free(new_row); return 0; }
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* Append the newly computed row to the cache */
        for (idx_t k = 1; k <= row; ++k)
            cache[cache_len++] = new_row[k];

        free(prev_row);
        prev_row = new_row;
        prev_len = row;
    }

    return cache[(size_t)n - 1];
}
