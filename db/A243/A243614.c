/*
 * A243614  Irregular triangular array of numerators of the positive rational
 *          numbers ordered as in Comments.
 * https://oeis.org/A243614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, frac
 * Formula:   not stated
 * Range:     u8 n <= 1500, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243614 – numerators of fractions in a Fibonacci‑based triangular array.
   Construction:
     row 1: (1/1)
     row 2: (2/1)
     for n ≥ 3:
        let r_{n-1} be the fractions of row n‑1 and r_{n-2} of row n‑2.
        row n consists of
          * for every f = a/b in r_{n-1}, the fraction (a+b)/b   (x+1)
          * for every f = a/b in r_{n-2}, the fraction a/(a+b)   (x/(x+1))
        the F(n) fractions are listed in decreasing order of the fractions.
   The sequence a(n) is the n‑th numerator in this row‑by‑row enumeration. */

#include <stdlib.h>
#include <stddef.h>

typedef struct {
    seq_t num;
    seq_t den;
} frac_t;

static seq_t *cache   = NULL;     /* all numerators produced so far */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static frac_t *row_prev2 = NULL;   /* row n‑2 */
static size_t   len_prev2 = 0;

static frac_t *row_prev1 = NULL;   /* row n‑1 */
static size_t   len_prev1 = 0;

static idx_t    rows_built = 0;    /* number of rows already built */

/* Ensure cache has space for 'needed' more elements. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_len + needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < cache_len + needed)
            new_cap = cache_len + needed;
        /* cast malloc result to required type */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* fatal error, abort */
        cache   = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined region, harmless */

    /* Generate rows until we have at least n terms. */
    while (cache_len < (size_t)n) {
        ++rows_built;

        if (rows_built == 1) { /* row 1 : (1/1) */
            frac_t *row = (frac_t *)malloc(sizeof(frac_t));
            row[0].num = (seq_t)1;
            row[0].den = (seq_t)1;
            ensure_cache_cap(1);
            cache[cache_len++] = row[0].num;

            row_prev1 = row;
            len_prev1 = 1;
            row_prev2 = NULL;
            len_prev2 = 0;
        }
        else if (rows_built == 2) { /* row 2 : (2/1) */
            frac_t *row = (frac_t *)malloc(sizeof(frac_t));
            row[0].num = (seq_t)2;
            row[0].den = (seq_t)1;
            ensure_cache_cap(1);
            cache[cache_len++] = row[0].num;

            /* shift rows: previous row1 becomes row n‑2. */
            if (row_prev2) free(row_prev2);
            row_prev2 = row_prev1;
            len_prev2 = len_prev1;

            row_prev1 = row;
            len_prev1 = 1;
        }
        else { /* rows ≥ 3 */
            size_t new_len = len_prev1 + len_prev2;
            frac_t *row = (frac_t *)malloc(new_len * sizeof(frac_t));

            /* (i) x+1 from row_prev1 */
            for (size_t i = 0; i < len_prev1; ++i) {
                const frac_t *f = &row_prev1[i];
                row[i].num = f->num + f->den;
                row[i].den = f->den;
            }
            /* (ii) x/(x+1) from row_prev2 */
            for (size_t i = 0; i < len_prev2; ++i) {
                const frac_t *f = &row_prev2[i];
                row[len_prev1 + i].num = f->num;
                row[len_prev1 + i].den = f->num + f->den;
            }
            /* Append numerators to cache */
            ensure_cache_cap(new_len);
            for (size_t i = 0; i < new_len; ++i)
                cache[cache_len++] = row[i].num;

            /* Shift rows for next iteration */
            if (row_prev2) free(row_prev2);
            row_prev2 = row_prev1;
            len_prev2 = len_prev1;

            row_prev1 = row;
            len_prev1 = new_len;
        }
    }

    return cache[(size_t)n - 1];
}
