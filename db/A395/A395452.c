/*
 * A395452  Triangle read by rows: T(n, k) = Sum_{j=0..n} (-1)^(n-j)*C(-j-1,
 *          -n-1)*E2(j, n-k), E2 the Eulerian numbers A201637.
 * https://oeis.org/A395452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) r = floor((sqrt(8n+1)-1)/2) (row index) c = n -
 *            r*(r+1)/2 (column index in row) T(r,c) = sum_{j=0..r} binom(r,j)
 *            * E2(j, r-c) where E2 is the Eulerian numbers of the second
 *            kind, defined by E2(0,0)=1 and E2(n,k) = (2*n-k-1)*E2(n-1,k-1) +
 *            (k+1)*E2(n-1,k)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"

static seq_t *cache_a = NULL;          /* sequence values a(n) */
static size_t cache_len = 0;           /* number of values stored */
static size_t cache_cap = 0;           /* capacity of cache_a */

static seq_t **e2_rows = NULL;         /* pointers to rows of E2 */
static size_t e2_rows_count = 0;       /* number of E2 rows computed */
static size_t e2_rows_cap = 0;         /* capacity of e2_rows */

/* ensure cache has values up to index n */
static void ensure_cached(idx_t n)
{
    while (cache_len <= (size_t)n)
    {
        /* compute next row r = e2_rows_count */
        size_t r = e2_rows_count;
        size_t row_len = r + 1;

        /* allocate new row for E2(r,*) */
        seq_t *row = (seq_t*)malloc(row_len * sizeof(seq_t));
        if (!row) { abort(); }

        if (r == 0)
        {
            row[0] = (seq_t)1;
        }
        else
        {
            seq_t *prev = e2_rows[r-1];
            for (size_t k = 0; k <= r; ++k)
            {
                seq_t term1 = 0;
                seq_t term2 = 0;
                if (k > 0)
                {
                    term1 = (seq_t)(2*r - k - 1) * prev[k-1];
                }
                if (k < r)
                {
                    term2 = (seq_t)(k + 1) * prev[k];
                }
                row[k] = term1 + term2;
            }
        }

        /* store the new row in e2_rows */
        if (e2_rows_cap <= e2_rows_count)
        {
            size_t new_cap = e2_rows_cap ? e2_rows_cap * 2 : 16;
            seq_t **tmp = (seq_t**)realloc(e2_rows, new_cap * sizeof(seq_t*));
            if (!tmp) { abort(); }
            e2_rows = tmp;
            e2_rows_cap = new_cap;
        }
        e2_rows[e2_rows_count] = row;
        e2_rows_count++;

        /* ensure cache has enough space */
        if (cache_cap < cache_len + row_len)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 256;
            while (new_cap < cache_len + row_len) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache_a, new_cap * sizeof(seq_t));
            if (!tmp) { abort(); }
            cache_a = tmp;
            cache_cap = new_cap;
        }

        /* compute T(r, k) for all k and append to cache */
        for (size_t k = 0; k <= r; ++k)
        {
            idx_t m = r - k;          /* m = r - k = n-k for this row */
            seq_t sum = 0;
            for (size_t j = m; j <= r; ++j)
            {
                seq_t bin = binom((idx_t)r, (idx_t)j);
                seq_t e = e2_rows[j][m];
                sum += bin * e;
            }
            cache_a[cache_len++] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cached(n);
    return cache_a[(size_t)n];
}
