/*
 * A247236  Triangle read by rows: T(n,k) appears in the transformation
 *          Sum_{k=0..n} (k+1)*x^k = Sum_{k=0..n} T(n,k)*(x+k)^k.
 * https://oeis.org/A247236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2) and c = n -
 *            r*(r+1)/2, 0 ≤ c ≤ r. The triangle T is defined by Sum_{k=0..n}
 *            (k+1)*x^k = Sum_{k=0..n} T(n,k)*(x+k)^k
 * Range:     i8 n <= 11, i16 n <= 30, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b) */

/* Local power helper for arbitrary‑precision seq_t */
static seq_t pow_seq_int(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp--)
        result *= base;
    return result;
}

/* Cache of pre‑computed rows of the triangle */
static seq_t **row_cache = NULL;
static idx_t cache_size  = 0;

/* Ensure that cache holds at least indices [0 .. r] */
static void ensure_cache(idx_t r)
{
    if (r < cache_size)
        return;
    idx_t new_size = (r < 16) ? 16 : r * 2;
    seq_t **new_cache = (seq_t **)realloc(row_cache, new_size * sizeof(seq_t *));
    if (!new_cache)
        abort();               /* out of memory */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = NULL;
    row_cache = new_cache;
    cache_size = new_size;
}

/* Compute the entire row r of the triangle and store in cache */
static void compute_row(idx_t r)
{
    seq_t *row = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!row)
        abort();               /* out of memory */

    row[r] = (seq_t)(r + 1);     /* T(r,r) = r+1 */

    /* Solve for j = r-1 … 0 */
    for (idx_t j = r; j-- > 0; )
    {
        seq_t sum = (seq_t)(j + 1);   /* RHS coefficient j+1 */
        for (idx_t k = j + 1; k <= r; ++k)
        {
            seq_t term = binomial(k, j);
            term *= pow_seq_int((seq_t)k, (idx_t)(k - j));
            term *= row[k];
            sum -= term;              /* subtract contributions of higher k */
        }
        row[j] = sum;
    }

    row_cache[r] = row;
}

/* Main sequence function requested by the assignment */
seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (n >= (r + 1) * (r + 2) / 2)
        ++r;

    /* Ensure the required row is computed and cached */
    ensure_cache(r);
    if (!row_cache[r])
        compute_row(r);

    /* Column index */
    idx_t tri = r * (r + 1) / 2;
    idx_t c   = n - tri;
    return row_cache[r][c];
}
