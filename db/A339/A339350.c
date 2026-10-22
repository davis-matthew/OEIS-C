/*
 * A339350  Triangle read by rows. T(n,k) = Sum_{j=0..k} binomial(k-j+2,
 *          2)*T(n-1, j), for n>=0, 0<=k<=n, with T(0,0)=1 and T(n,n)=0 for
 *          n>0.
 * https://oeis.org/A339350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 101, u64 n <= 297,
 *            u128 n <= 1030, big any n
 * Helpers:   binomial, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  
   T(0,0) = 1,   T(n,n) = 0 for n>0  
   T(n,k) = Σ_{j=0..k} binomial(k-j+2, 2) * T(n-1,j)   (0 ≤ k ≤ n)  
   a(n)   is the flattening of the triangle T(n,k) read by rows. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    /* static cache of triangle rows */
    static seq_t **rows = NULL;          /* array of pointers to rows   */
    static size_t rows_alloc = 0;        /* capacity of rows array      */
    static size_t rows_used = 0;         /* number of rows stored       */
    static size_t total_elements = 0;    /* total number of elements    */

    /* allocate the rows array on first call */
    if (rows == NULL) {
        rows_alloc = 8;
        rows = (seq_t**)malloc(rows_alloc * sizeof(*rows));
    }

    /* build rows until we have enough elements to answer index n */
    while (total_elements <= (size_t)n) {
        idx_t r = (idx_t)rows_used;              /* index of new row */
        size_t len = (size_t)r + 1;              /* number of entries in the row */
        seq_t *row = seq_alloc(len);             /* allocate storage for the row */

        if (r == 0) {
            /* base case T(0,0) = 1 */
            row[0] = (seq_t)1;
        } else {
            /* compute T(r,k) for k=0..r using the recurrence */
            for (idx_t k = 0; k <= r; ++k) {
                if (k == r) {
                    /* T(r,r) = 0 for r>0 */
                    row[k] = (seq_t)0;
                } else {
                    seq_t sum = (seq_t)0;
                    for (idx_t j = 0; j <= k; ++j) {
                        seq_t coeff = binomial((idx_t)(k - j + 2), (idx_t)2);
                        sum += coeff * rows[rows_used - 1][j];
                    }
                    row[k] = sum;
                }
            }
        }

        /* Store the new row in the cache */
        if (rows_used == rows_alloc) {
            rows_alloc *= 2;
            rows = (seq_t**)realloc(rows, rows_alloc * sizeof(*rows));
        }
        rows[rows_used++] = row;
        total_elements += len;
    }

    /* locate the row and offset corresponding to index n */
    size_t cumulative = 0;
    size_t r = 0;
    while (cumulative + (r + 1) <= (size_t)n) {
        cumulative += r + 1;
        ++r;
    }
    size_t offset = (size_t)n - cumulative;
    return rows[r][offset];
}
