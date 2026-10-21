/*
 * A247488  Triangle read by rows: T(n,k) = abs(C(n-k, k)*hypergeom([k-n/2,
 *          k-n/2+1/2], [1], -4)).
 * https://oeis.org/A247488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = abs( C(n-k,k) * * hypergeom([k-n/2, k-n/2+1/2],[1],-4)
 *            ), * a(n) = flatten triangle read by rows, offset 0
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   abs_int128, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"      /* declares seq_t binomial(idx_t n, idx_t k); */
#include "../utils/abs_int128.h"

/* cache of already computed sequence values, length = next_needed_index */
static seq_t *cache = NULL;
static size_t cache_len = 0;       /* amount of entries stored */

/* row that we have already computed up to (next row will start at this index) */
static idx_t rows_computed = 0;

/* next sequence index that has yet to be computed */
static idx_t next_needed_index = 0;

/* triangular row index r starts here: calculate row r (0‑based)
 * number of entries in that row is floor(r/2)+1
 */
static seq_t triangle_row_entry(idx_t r, idx_t k)
{
    idx_t m = r >> 1;              /* floor(r/2)   */
    idx_t t = m - k;               /* loop limit   */

    /* binomial factor */
    seq_t bval = binomial(r - k, k);
    __int128 term = (__int128)bval;     /* signed 128 */
    __int128 sum  = term;               /* running sum */

    /* recurrence for terms of the hypergeometric series */
    for (idx_t j = 1; j <= t; ++j) {
        int64_t a_or_c;                /* a for even rows, c for odd rows */
        int64_t factor, denom;
        if ((r & 1) == 0) {           /* even r : a = k - m          */
            a_or_c = (int64_t)k - (int64_t)m;
            factor = 2 * (a_or_c + (int64_t)j - 1)
                   * (2 * a_or_c + 2 * (int64_t)j - 1);
        } else {                      /* odd r : c = k - m           */
            a_or_c = (int64_t)k - (int64_t)m;
            factor = 2 * (a_or_c + (int64_t)j - 1)
                   * (2 * a_or_c + 2 * (int64_t)j - 3);
        }
        denom = (int64_t)j * (int64_t)j;

        /* multiply and divide, then flip sign */
        term = term * ((__int128)factor) / denom;
        term = -term;
        sum  += term;
    }

    return abs_int128(sum);
}

/* extend the cache so that index n can be accessed */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);          /* memory failure */
    cache = new_cache;
    cache_len = new_len;
}

/* main OEIS entry routine */
seq_t a(idx_t n)
{
    /* compute rows until the requested index is available */
    while (next_needed_index <= n) {
        /* produce all entries of the next triangle row */
        idx_t r = rows_computed;
        idx_t kmax = r >> 1;          /* floor(r/2) */
        for (idx_t k = 0; k <= kmax; ++k) {
            ensure_cache(next_needed_index);
            cache[next_needed_index++] = triangle_row_entry(r, k);
        }
        ++rows_computed;
    }
    return cache[n];
}
