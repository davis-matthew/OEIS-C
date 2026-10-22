/*
 * A305556  Irregular triangle read by rows: T(n,k) is the number of
 *          superdiagonal bargraphs with area n and with k columns.
 * https://oeis.org/A305556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   T(n,k) = binomial(n‑1‑k*(k‑1)/2, k‑1). a(n) enumerates the
 *            triangle row‑wise: a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), … The
 *            function maps the linear index n to the unique pair (row,column)
 *            and returns the corresponding binomial value
 * Range:     u8 n <= 53, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* integer square root of an unsigned 64‑bit integer, floor of sqrt(x) */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32); /* sqrt(2^64-1) < 2^32 */
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* invalid index, but harmless */

    /* static cache of cumulative row lengths: cum[i] = sum of row lengths up to row i */
    static idx_t *cum = NULL;
    static size_t cap = 0;            /* allocated capacity of cum array */
    static idx_t rows_filled = 0;     /* number of rows currently stored (cum[0] .. cum[rows_filled]) */

    /* initialise cache on first call */
    if (!cum)
    {
        cap = 64;
        cum = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!cum)
            return (seq_t)0;           /* allocation failure */
        cum[0] = 0;                    /* before row 1 */
        rows_filled = 0;
    }

    /* extend the cache until it covers the requested index n */
    while (cum[rows_filled] < n)
    {
        /* next row number (1‑based) is rows_filled + 1 */
        idx_t r = rows_filled + 1;
        /* maximum column count for this row */
        idx_t t = isqrt_u64(1 + 8 * r);
        idx_t row_len = (t - 1) / 2;     /* floor((sqrt(1+8r)-1)/2) */
        if (row_len == 0)                /* should not happen for r>=1 */
            row_len = 1;

        /* grow the array if necessary */
        if (rows_filled + 1 >= cap)
        {
            size_t new_cap = cap << 1;
            idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            if (!tmp)
                return (seq_t)0;       /* allocation failure */
            cum = tmp;
            cap = new_cap;
        }

        cum[rows_filled + 1] = cum[rows_filled] + row_len;
        ++rows_filled;
    }

    /* locate the row and column for the linear index n */
    idx_t row = rows_filled;                    /* 1‑based row number */
    idx_t col = n - cum[row - 1];                /* 1‑based column number */

    /* compute T(row, col) = binomial(row‑1‑col*(col‑1)/2, col‑1) */
    idx_t sub = col * (col - 1) / 2;
    idx_t base = (row - 1) - sub;                /* argument for binomial */
    if (base < col - 1)
        return (seq_t)0;                        /* out of range, should not occur */

    return binomial(base, col - 1);
}
