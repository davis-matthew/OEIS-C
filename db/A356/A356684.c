/*
 * A356684  a(n) = (n-1)*a(n-1) - n*a(n-2), with a(1) = a(2) = -1.
 * https://oeis.org/A356684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-1)*a(n-1) - n*a(n-2), a(1)=a(2)=-1
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/*
 * Computes the nth term of OEIS sequence A356684.
 *
 * The function is memoised: it keeps a static array of previously
 * computed terms and extends it when a larger n is requested.  As a
 * consequence each sequence value is computed only once, regardless
 * of the order or number of calls.
 */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (in number of terms) */
    static idx_t  size  = 0;   /* number of terms actually computed */

    if (n == 0)                  /* undefined for 0, return 0 */
        return 0;

    /* ensure cache has space for at least n terms */
    if ((size_t)n > cap) {
        size_t newcap = cap > 0 ? cap : 8;
        while (newcap < (size_t)n) newcap <<= 1;
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* initialise the first two terms if not already done */
    if (size < 2) {
        cache[0] = (seq_t)-1;          /* a(1) */
        cache[1] = (seq_t)-1;          /* a(2) */
        size    = 2;
    }

    /* if the requested n has already been computed, return it */
    if (n <= size)
        return cache[n-1];

    /* compute missing terms iteratively, extending the cache */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t prev1 = cache[i-2];      /* a(i-1) */
        seq_t prev2 = cache[i-3];      /* a(i-2) */
        cache[i-1] = ((seq_t)(i-1)) * prev1
                     - ((seq_t)i) * prev2;
    }

    size = n;                          /* we now have all terms up to n */
    return cache[n-1];
}
