/*
 * A300347  Number of nX5 0..1 arrays with every element equal to 1 or 2
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 55, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements the recurrence from OEIS A300347.  
   Empirical recurrence: 
     a(n) = 5*a(n-1) -8*a(n-2) +4*a(n-3) +a(n-6) -3*a(n-7)
            +4*a(n-8) -2*a(n-9) +a(n-10) -a(n-11) -a(n-14) +a(n-15)  
   for n > 19, with initial values a(1) … a(19) below.
*/
#include <stdlib.h>

static const seq_t init_vals[19] = {
    3ULL,      37ULL,     57ULL,     152ULL,     306ULL,
    729ULL,   1496ULL,   3223ULL,   6893ULL,   14847ULL,
   31957ULL,  68842ULL,  148439ULL, 320256ULL,  691019ULL,
 1491306ULL, 3218713ULL, 6947412ULL, 14996127ULL
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 32;                          /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* give up on allocation failure */
        for (size_t i = 0; i < 19; ++i)
            cache[i] = init_vals[i];
        cache_len = 19;
    }

    /* if requested term is already known, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend cache if necessary */
    while (cache_len < (size_t)n) {
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute a(curr) using the recurrence */
        idx_t curr = (idx_t)(cache_len + 1);   /* current index to compute */
        seq_t val;

        /* indices in the recurrence are based on a(curr-1)...a(curr-15) */
        val = 5 * cache[curr - 2]           /* a(curr-1) */
            - 8 * cache[curr - 3]           /* a(curr-2) */
            + 4 * cache[curr - 4]           /* a(curr-3) */
            +   cache[curr - 7]           /* a(curr-6) */
            - 3 * cache[curr - 8]           /* a(curr-7) */
            + 4 * cache[curr - 9]           /* a(curr-8) */
            - 2 * cache[curr -10]           /* a(curr-9) */
            +   cache[curr -11]           /* a(curr-10) */
            -   cache[curr -12]           /* a(curr-11) */
            -   cache[curr -15]           /* a(curr-14) */
            +   cache[curr -16];          /* a(curr-15) */

        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n - 1];
}
