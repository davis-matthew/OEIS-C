/*
 * A268053  Number of n X 5 0..2 arrays with new values introduced in each row
 *          and column in sequential order starting with zero.
 * https://oeis.org/A268053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 62*a(n-1) - 969*a(n-2) + 4568*a(n-3) - 5740*a(n-4),
 * a(1) = 1, a(2) = 16, a(3) = 422, a(4) = 14554.
 * This implementation memoises results in an array so that
 * successive calls are O(1) after the first evaluation of each needed n.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of slots (including index 0 unused) */

    if (n < 1) {
        /* offset is 1; behaviour for n < 1 is irrelevant */
        return (seq_t)0;
    }

    /* ensure cache is large enough for n */
    if (n >= cache_len) {
        idx_t new_len = (cache_len == 0) ? 5 : cache_len * 2;
        while (new_len <= n) {
            new_len <<= 1;
        }
        cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        cache_len = new_len + 1;

        /* initialise the first four terms once */
        if (cache_len > 5) { /* first initialisation */
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)16;
            cache[3] = (seq_t)422;
            cache[4] = (seq_t)14554;
        } else { /* first allocation may be smaller */
            cache[0] = (seq_t)0;        /* unused */
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)16;
            cache[3] = (seq_t)422;
            cache[4] = (seq_t)14554;
        }
    }

    /* compute any missing values up to n */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t term1 = cache[i-1] * (seq_t)62;
        seq_t term2 = cache[i-3] * (seq_t)4568;
        seq_t sum   = term1 + term2;

        seq_t term3 = cache[i-2] * (seq_t)969;
        seq_t term4 = cache[i-4] * (seq_t)5740;
        seq_t sub   = term3 + term4;

        cache[i] = sum - sub;   /* recurrence guarantees non‑negative result */
    }

    return cache[n];
}
