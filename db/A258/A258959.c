/*
 * A258959  Number of (n+2) X (1+2) 0..1 arrays with no 3 X 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 0 or 3 and
 *          no column sum 0 or 3.
 * https://oeis.org/A258959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=66, a(2)=158, a(3)=214, a(4)=462, a(5)=676, a(6)=1374,
 *            a(7)=2040, a(8)=4104, a(9)=6136. For n>9: a(n) = -a(n-1) +
 *            3*a(n-2) + 3*a(n-3). Values are memoised for fast successive
 *            calls
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 33, u64 n <= 73, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms, index 0 unused */
static idx_t cache_max = 0;      /* largest index stored in cache */

/* initial values for 1..9 */
static const seq_t init_vals[10] = {
    0ULL,          /* unused at index 0 */
    66ULL, 158ULL, 214ULL, 462ULL, 676ULL,
    1374ULL, 2040ULL, 4104ULL, 6136ULL
};

/* expand the cache to hold at least indices 1..n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;                      /* already available */

    idx_t new_max = n;
    seq_t *tmp = NULL;

    if (cache == NULL) {              /* first allocation */
        tmp = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
        if (!tmp)
            return;                  /* allocation failure – caller sees 0 */
    } else {                          /* grow existing array */
        tmp = (seq_t *)realloc(cache,
                               (new_max + 1) * sizeof(seq_t));
        if (!tmp)
            return;                  /* allocation failure – caller sees 0 */
    }

    cache = tmp;

    /* initialise new elements */
    idx_t i;
    for (i = cache_max + 1; i <= new_max; ++i) {
        if (i <= 9) {
            cache[i] = init_vals[i];                 /* base terms */
        } else {
            /* a(i) = -a(i-1) + 3*a(i-2) + 3*a(i-3) */
            seq_t term = cache[i - 2];
            term *= (seq_t)3;
            term += cache[i - 3] * ((seq_t)3);
            term -= cache[i - 1];
            cache[i] = term;
        }
    }

    cache_max = new_max;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* sequence not defined for n=0 */

    ensure_cache(n);
    if (n > cache_max)         /* allocation failure */
        return (seq_t)0;
    return cache[n];
}
