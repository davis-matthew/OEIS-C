/*
 * A386441  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 27.
 * https://oeis.org/A386441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) mod 27, where i*(i+1)/2 <= n <
 *            (i+1)*(i+2)/2 and j = n - i*(i+1)/2
 * Range:     u8 n <= 10010, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 121 terms through n = 9700 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t **rows = NULL;      /* rows[i] points to the i‑th row (size i+1) */
static idx_t rows_len = 0;       /* number of rows currently allocated */

static void ensure_rows(idx_t r)
{
    if (rows_len > r)
        return;                     /* already have enough rows */

    seq_t **tmp = (seq_t **)realloc(rows, sizeof(seq_t *) * ((size_t)r + 1));
    if (!tmp)
        return;                    /* allocation failure – leave unchanged */
    rows = tmp;

    for (idx_t i = rows_len; i <= r; ++i)
    {
        rows[i] = seq_alloc_new_array((size_t)(i + 1));
        if (!rows[i])
            return;                 /* allocation failure – abort */
        rows[i][0] = (seq_t)1;
        rows[i][i] = (seq_t)1;
        for (idx_t k = 1; k < i; ++k)
        {
            seq_t v = rows[i - 1][k - 1] + rows[i - 1][k];
            rows[i][k] = v % (seq_t)27;
        }
    }
    rows_len = r + 1;
}

seq_t a(idx_t n)
{
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    idx_t j = n - i * (i + 1) / 2;
    ensure_rows(i);
    return rows[i][(size_t)j];
}
