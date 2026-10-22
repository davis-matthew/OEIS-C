/*
 * A362308  Triangle read by rows. Number of perfect matchings by number of
 *          connected components.
 * https://oeis.org/A362308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Term of OEIS A362308
   Triangle T(n,k) defined by
       T(0,0)=1,
       T(n,0)=0 for n>0,
       T(n,n)=1,
       T(n,k) = T(n,k-1) - T(n-1,k-2) - ((2*n - k - 1)/(k-1)) * T(n-1,k-1)   for k>1
   a(n) is the triangle read by rows (flattened). */

/* Required utilities */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) */

static seq_t *cache = NULL;          /* flattened triangle */
static size_t cache_len = 0;          /* number of cached entries */
static size_t cache_cap = 0;          /* allocated cache capacity */

static seq_t *prev_row = NULL;       /* previous row array */
static idx_t  prev_len = 0;          /* length of previous row (prev_len = r) */

/* Ensure capacity to store at least (len_needed) more elements. */
static void ensure_cache(size_t len_needed)
{
    if (cache_len + len_needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : len_needed;
    while (cache_len + len_needed > new_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialisation on first call */
    if (cache == NULL)
    {
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        prev_row = NULL;
        prev_len = 0;
    }

    /* Compute rows until the cache contains index n */
    while (cache_len <= (size_t)n)
    {
        idx_t r = (idx_t)(cache_len == 0 ? 0 :
                          cache_len == 1 ? 0 : /* already have row 0 */ 1); /* placeholder; unused */
        r = (idx_t)(cache_len ? 0 : 0); /* will be corrected below */
        r = (idx_t)(cache_len == 0 ? 0
            : (cache_len - 1) + 1); /* compute row index from cached entries:
                                      after row r-1 we have cache_len entries = sum_{i=0}^{r-1}(i+1) */
        /* The above simplifies to: */
        if (cache_len == 0) r = 0;
        else r = (idx_t)(0);
        /* Actually simpler: We keep a separate static counter of processed rows */
        /* Let's maintain processed_rows variable */
        static idx_t processed_rows = 0;
        r = processed_rows;
        idx_t row_len = r + 1;
        seq_t *row = (seq_t *)malloc(row_len * sizeof(seq_t));
        if (row == NULL)
            abort();  /* memory allocation failed */

        if (r == 0)
        {
            row[0] = 1;
        }
        else
        {
            row[0] = 0;            /* T(r,0) = 0 for r > 0 */

            /* Compute T(r,1) via the formula
               T(r,1) = 1 + Σ_{k=2..r} [ T(r-1,k-2) + ((2r - k -1)/(k-1)) * T(r-1,k-1) ] */

            seq_t sumC = 0;
            for (idx_t k = 2; k <= r; ++k)
            {
                seq_t term1 = prev_row[k - 2];
                seq_t num   = 2 * r - k - 1;
                seq_t denom = k - 1;
                seq_t prod = seq_mul(prev_row[k - 1], (seq_t)num);
                seq_t term2 = prod / (seq_t)denom;
                sumC += term1 + term2;
            }
            seq_t T1 = 1 + sumC;
            row[1] = T1;

            /* Compute remaining entries using recurrence */
            for (idx_t k = 2; k <= r; ++k)
            {
                seq_t num   = 2 * r - k - 1;
                seq_t denom = k - 1;
                seq_t prod = seq_mul(prev_row[k - 1], (seq_t)num);
                seq_t term = prod / (seq_t)denom;
                seq_t val = row[k - 1] - prev_row[k - 2] - term;
                row[k] = val;
            }
        }

        /* Store computed row into cache */
        ensure_cache(row_len);
        for (idx_t i = 0; i < row_len; ++i)
            cache[cache_len++] = row[i];

        /* Prepare for next iteration */
        free(prev_row);
        prev_row = row;
        prev_len = row_len;
        processed_rows = r + 1;
    }

    return cache[n];
}
