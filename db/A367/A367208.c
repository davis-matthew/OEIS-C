/*
 * A367208  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 1 + 3*x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >= 3, where u = p(2,x), v =
 *          1 - x - x^2.
 * https://oeis.org/A367208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑wise enumeration of the coefficients of the *
 *            strong divisibility sequence of polynomials * p(1,x) = 1 *
 *            p(2,x) = 1 + 3*x * p(n,x) = (1 + 3*x)*p(n‑1,x) + (1 – x –
 *            x²)*p(n‑2,x) (n ≥ 3) * The coefficients of p(n,x) form the
 *            triangular array T(n,k); a(n) is * read row‑by‑row from thi...
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t mul_small(seq_t x, unsigned int m);

static seq_t *cache = NULL;          /* memoised terms a(1)…a(cache_size) */
static idx_t   cache_size = 0;       /* number of terms stored in cache   */
static idx_t   cache_cap  = 0;       /* allocated capacity of cache      */

/* buffers holding the last two rows of the polynomial coefficients */
static seq_t *row_prev_prev = NULL;  /* p(n‑2,x) */
static idx_t  len_prev_prev = 0;
static seq_t *row_prev     = NULL;   /* p(n‑1,x) */
static idx_t  len_prev     = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; 0 is undefined */

    /* --- initialisation of memoisation tables --- */
    if (cache == NULL) {
        /* start with a small capacity and grow as needed */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        /* row 1: p(1,x) = 1 */
        len_prev_prev = 1;
        row_prev_prev = (seq_t *)malloc(sizeof(seq_t));
        row_prev_prev[0] = 1;
        cache[cache_size++] = row_prev_prev[0];

        /* row 2: p(2,x) = 1 + 3*x */
        len_prev = 2;
        row_prev = (seq_t *)malloc(2 * sizeof(seq_t));
        row_prev[0] = 1;
        row_prev[1] = 3;
        cache[cache_size++] = row_prev[0];
        cache[cache_size++] = row_prev[1];
    }

    /* --- extend memoisation until we have at least n terms --- */
    while (cache_size < n) {
        idx_t r = len_prev + 1;              /* current row number */
        idx_t len_curr = r;
        seq_t *row_curr = (seq_t *)malloc(len_curr * sizeof(seq_t));

        for (idx_t k = 0; k < len_curr; ++k) {
            seq_t pos = 0, neg = 0;

            /* positive contributions from u*p(n‑1) = (1+3x)*p(n‑1) */
            if (k < len_prev)            pos += row_prev[k];
            if (k >= 1 && k-1 < len_prev) {
                seq_t tmp = row_prev[k-1];
                pos += tmp; pos += tmp; pos += tmp;   /* 3 * row_prev[k-1] */
            }
            if (k < len_prev_prev)       pos += row_prev_prev[k];

            /* negative contributions from v*p(n‑2) = (1‑x‑x²)*p(n‑2) */
            if (k >= 1 && k-1 < len_prev_prev) neg += row_prev_prev[k-1];
            if (k >= 2 && k-2 < len_prev_prev) neg += row_prev_prev[k-2];

            /* final coefficient (pos ≥ neg in the recurrence) */
            seq_t coeff = pos - neg;
            row_curr[k] = coeff;

            /* store coefficient in the cache */
            if (cache_size == cache_cap) {
                cache_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = coeff;
        }

        /* shift rows for next iteration */
        free(row_prev_prev);
        row_prev_prev = row_prev;
        len_prev_prev = len_prev;

        row_prev = row_curr;
        len_prev = len_curr;
    }

    return cache[n-1];
}
