/*
 * A246798  Triangle read by rows: T(n,k) is the coefficient A_k in the
 *          transformation Sum_{k=0..n} (k+1)*x^k = Sum_{k=0..n} A_k*(x+3)^k.
 * https://oeis.org/A246798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 29, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns terms of OEIS sequence A246798.
   The triangle T(n,k) is obtained by converting the polynomial
   sum_{k=0}^{n} (k+1)x^k into sum_{k=0}^{n} A_k (x+3)^k.
   The coefficients A_k are computed by a recurrence that
   avoids explicit large powers of 3.  The overall sequence
   is simply the triangle read by rows.  The routine
   caches computed rows so that repeated calls are cheap. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* flattened sequence terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* current capacity of cache */
    static idx_t rows_done = 0;      /* number of rows that have been computed (row indices 0..rows_done-1) */

    if (n < cache_len)                /* already know this term */
        return cache[n];

    /* Grow the cache until it contains at least n+1 terms */
    while (cache_len <= n) {
        idx_t r = rows_done;                  /* next row index to generate */
        size_t row_size = (size_t)r + 1;      /* number of elements in the new row */
        seq_t *row = (seq_t *)malloc(row_size * sizeof(seq_t));
        if (!row) return 0;                   /* out of memory protection */
        for (size_t k = 0; k < row_size; ++k) row[k] = 0;

        /* Build the current row by summing contributions from every
           term (s+1)x^s of the source polynomial. */
        for (idx_t s = 0; s <= r; ++s) {
            __int128 term = (__int128)(s + 1);   /* contribution for k = s (multiplied by (-3)^0) */
            row[(size_t)s] += (seq_t)term;      /* add to A_s */

            /* Walk downwards to compute contributions for k = s-1 ... 0
               using the recurrence
                   T_{k-1} = T_k * (k) * (-3) / (s - (k-1))   (k runs from s downwards).   */
            idx_t k = s;
            while (k > 0) {
                idx_t nxt = k - 1;               /* next k value */
                term = term * (__int128)(nxt + 1) * (__int128)(-3);
                term /= (__int128)(s - nxt);
                row[(size_t)nxt] += (seq_t)term;
                k = nxt;
            }
        }

        /* Ensure sufficient capacity in the cache */
        size_t needed = cache_len + row_size;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* out of memory protection */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Append the new row to the cache */
        for (size_t k = 0; k < row_size; ++k)
            cache[cache_len + k] = row[k];
        cache_len += row_size;
        rows_done++;                        /* one more row completed */

        free(row);
    }

    /* Now the cache contains the desired term */
    return cache[n];
}
