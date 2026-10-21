/*
 * A263730  Irregular triangle read by rows in which row n > 1 lists k such
 *          that (k^2 + k*n)/(k + 1) is an integer.
 * https://oeis.org/A263730
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the (n‑2)-th element of the flattened * sequence of
 *            k‑values from the irregular triangle. * That triangle lists, for
 *            each n>1, all k such that (k^2 + k*n)/(k+1) is integer, * which
 *            is equivalent to k+1 | (n-1). Thus each row n lists k = d-1 for
 *            all * divisors d of n-1 in increasing order....
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global cache for divisors.  The divisor table is built up to the
 * maximum value of n-1 seen so far.  Each entry stores the sorted
 * list of divisors of the index value.  The maximum number of
 * divisors for m <= 10 000 is well below 128, so a fixed buffer
 * of that size is sufficient. */
static size_t  div_cache_limit   = 0;          /* largest m already cached */
static unsigned int **div_cache    = NULL;     /* array of pointers to divisor lists */
static unsigned char *div_cache_len = NULL;    /* number of divisors in each entry */

/* Utility to rebuild the divisor cache up to 'limit' (inclusive). */
static void rebuild_divisor_cache(size_t limit)
{
    /* Grow the cache to accommodate indices up to 'limit'. */
    div_cache = (unsigned int**)realloc(div_cache, (limit + 1) * sizeof(unsigned int*));
    div_cache_len = (unsigned char*)realloc(div_cache_len, (limit + 1) * sizeof(unsigned char));

    /* For each new m, compute its divisors in ascending order. */
    for (size_t m = div_cache_limit + 1; m <= limit; ++m) {
        /* Allocate a temporary vector large enough for any divisor list.
         * For m <= 10 000 the maximum number of divisors is < 128. */
        unsigned int *temp = (unsigned int*)malloc(128 * sizeof(unsigned int));
        size_t cnt = 0;

        /* Small divisors up to sqrt(m). */
        for (size_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                temp[cnt++] = (unsigned int)d;
                if (d * d != m)
                    temp[cnt++] = (unsigned int)(m / d);
            }
        }

        /* Sort ascending.  The list is tiny, so insertion sort is fine. */
        for (size_t i = 1; i < cnt; ++i) {
            unsigned int key = temp[i];
            size_t j = i;
            while (j > 0 && temp[j - 1] > key) {
                temp[j] = temp[j - 1];
                --j;
            }
            temp[j] = key;
        }

        /* Store the sorted divisor list. */
        div_cache[m] = temp;
        div_cache_len[m] = (unsigned char)cnt;
    }
    div_cache_limit = limit;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                           /* before the defined offset */

    /* The target is the (n-1)-th element in the flattened sequence. */
    idx_t target = n - 1;
    idx_t counter = 0;

    /* Ensure divisors are cached up to at least (n-1). */
    if (div_cache_limit < (size_t)(n - 1))
        rebuild_divisor_cache((size_t)(n - 1));

    /* Scan rows in order, counting terms until reaching 'target'. */
    for (idx_t row = 2; row <= n; ++row) {
        size_t m = (size_t)(row - 1);      /* value whose divisors give the row */
        unsigned int *dlist = div_cache[m];
        unsigned char dcnt  = div_cache_len[m];

        for (unsigned char idx = 0; idx < dcnt; ++idx) {
            ++counter;
            if (counter == target) {
                /* Return k = d-1 as seq_t. */
                return (seq_t)(dlist[idx] - 1);
            }
        }
    }

    /* Should never reach here for valid n, but return 0 as a safeguard. */
    return 0;
}
