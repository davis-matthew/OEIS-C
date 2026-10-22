/*
 * A308520  Expansion of e.g.f. exp(x)*(1 + x + x^2/2)*(sec(x) + tan(x)).
 * https://oeis.org/A308520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Boustrophedon transform of A000124 (central polygonal numbers).
 * Construct a triangular array T with T[0][0] = A000124(0) = 1.
 * For n ≥ 1:
 *     T[n][0] = A000124(n) = n*(n+1)/2 + 1
 *     For j = 1 .. n:
 *         T[n][j] = T[n][j-1] + T[n-1][n-j]
 * Then a(n) = T[n][n].
 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of previous rows and a(n) values */
    static seq_t *prev_row = NULL;   /* previous row (length prev_len) */
    static size_t   prev_len = 0;
    static seq_t *cache = NULL;      /* cache of a(k) values, size cache_size */
    static size_t   cache_size = 0;
    static idx_t    cached_max = (idx_t)-1;   /* highest index computed so far */

    /* initialise for the first call */
    if (cached_max == (idx_t)-1) {
        prev_len = 1;
        prev_row = (seq_t *)malloc(sizeof(seq_t));
        if (!prev_row) return 0;          /* out of memory */
        prev_row[0] = 1;                  /* A000124(0) */

        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = prev_row[0];           /* a(0) */
        cached_max = 0;
    }

    /* if the requested n is already cached, return it */
    if (n <= cached_max) return cache[n];

    /* grow the cache array if necessary */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute rows up to n */
    for (idx_t m = cached_max + 1; m <= n; ++m) {
        size_t curr_len = (size_t)m + 1;
        seq_t *curr = (seq_t *)malloc(curr_len * sizeof(seq_t));
        if (!curr) return 0;

        /* base value: A000124(m) = m*(m+1)/2 + 1 */
        seq_t base = ((seq_t)m * ((seq_t)m + 1)) / 2 + 1;
        curr[0] = base;

        /* fill the row */
        for (size_t j = 1; j <= m; ++j) {
            curr[j] = curr[j - 1] + prev_row[m - j];
        }

        /* store the final element as a(m) */
        cache[m] = curr[m];

        /* advance to next row */
        free(prev_row);
        prev_row = curr;
        prev_len = curr_len;
        cached_max = m;
    }

    return cache[n];
}
