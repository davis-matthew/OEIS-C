/*
 * A286718  Triangle read by rows: T(n, k) is the Sheffer triangle ((1 -
 *          3*x)^(-1/3), (-1/3)*log(1 - 3*x)). A generalized Stirling1
 *          triangle.
 * https://oeis.org/A286718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 54, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A286718 – flattened Sheffer triangle:
   T(n,k) = T(n-1,k-1) + (3*n-2)*T(n-1,k),
   T(0,0)=1, T(n,k)=0 for k>n.
   a(n) is the element T(row,col) where n = 0,1,2,… is unflattened.
   Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=4, … (OEIS b‑file)
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/triangular.h"

/* Integer square root for unsigned 128‑bit values */
static inline idx_t isqrt_u128(__int128 x)
{
    idx_t r = 0;
    idx_t bit = 1ull << 62;           /* highest bit that can appear */
    while (bit) {
        idx_t nxt = r + bit;
        if ((__int128)nxt * nxt <= x) r = nxt;
        bit >>= 1;
    }
    return r;
}

/* Triangular number: T(r) = r*(r+1)/2 */

seq_t a(idx_t n)
{
    /* Determine row and column of the flattened index n */
    idx_t d   = isqrt_u128((__int128)8 * n + 1);
    idx_t row = (d - 1) >> 1;                  /* (d-1)/2 */
    idx_t col = n - triangular(row);

    /* Static cache of all triangle entries up to the largest needed row */
    static seq_t *cache = NULL;
    static idx_t cached_rows = 0;              /* number of rows already computed */

    if (cache == NULL) {                       /* first call – initialise */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;                   /* T(0,0) */
        cached_rows = 1;
    }

    /* Expand cache until it contains at least 'row' */
    if (row + 1 > cached_rows) {
        /* New total number of elements needed */
        size_t new_total = (size_t)triangular(row + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_total * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;

        /* Compute missing rows */
        for (idx_t i = cached_rows; i <= row; ++i) {
            idx_t prev_offset = triangular(i - 1);
            idx_t curr_offset = triangular(i);

            seq_t factor = (seq_t)3 * (seq_t)i - (seq_t)2;

            /* First element of the new row */
            cache[curr_offset] = factor * cache[prev_offset];

            /* Remaining elements of the row */
            for (idx_t j = 1; j <= i; ++j) {
                seq_t prev_left  = cache[prev_offset + j - 1];
                seq_t prev_center = (j < i)
                                    ? cache[prev_offset + j]
                                    : (seq_t)0;
                cache[curr_offset + j] = prev_left + factor * prev_center;
            }
        }
        cached_rows = row + 1;
    }

    /* Return the required triangle entry */
    idx_t offset = triangular(row);
    return cache[offset + col];
}
