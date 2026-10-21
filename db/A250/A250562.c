/*
 * A250562  Number of length 3+2 0..n arrays with the sum of second
 *          differences multiplied by some arrangement of +-1 equal to zero.
 * https://oeis.org/A250562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=a(n-1)-a(n-2)+2*a(n-3)+a(n-5)-2*a(n-7)-3*a(n-9)+a(n-10)
 -a(n-11)+3*a(n-12)+2*a(n-14)-a(n-16)-2*a(n-18)+a(n-19)-a(n-20)+a(n-21)
 Initial values: a(1)=14, a(2)=83, a(3)=302, a(4)=761, a(5)=1648,
 a(6)=3125, a(7)=5446, a(8)=8843, a(9)=13662, a(10)=20173,
 a(11)=28836, a(12)=39973, a(13)=54102, a(14)=71647, a(15)=93210,
 a(16)=119221, a(17)=150416, a(18)=187305, a(19)=230650,
 a(20)=281051, a(21)=339378 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0] holds a(1) */
static idx_t cached = 0;        /* number of computed terms in cache */

static const seq_t init_vals[21] = {
    14, 83, 302, 761, 1648, 3125, 5446, 8843, 13662,
    20173, 28836, 39973, 54102, 71647, 93210, 119221,
    150416, 187305, 230650, 281051, 339378
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset; not used in problem */

    /* Return pre‑computed initial terms directly */
    if (n <= 21) return init_vals[n - 1];

    /* Allocate or grow the cache as needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(n * sizeof(seq_t));
        if (cache == NULL) abort();
        cached = 0;
    } else if (cached < n) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (cache == NULL) abort();
    }

    /* Ensure the first 21 terms are in the cache */
    if (cached < 21) {
        idx_t upto = (n < 21 ? n : 21);
        for (idx_t i = cached; i < upto; ++i) cache[i] = init_vals[i];
        cached = upto;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t k = cached + 1; k <= n; ++k) {
        /* Summation of the positive coefficient terms */
        seq_t pos = cache[k - 2];                       /* + a(k-1)  */
        pos += 2 * cache[k - 4];                       /* + 2*a(k-3)*/
        pos += cache[k - 6];                           /* + a(k-5)  */
        pos += cache[k - 11];                          /* + a(k-10)*/
        pos += 3 * cache[k - 13];                      /* + 3*a(k-12)*/
        pos += 2 * cache[k - 15];                      /* + 2*a(k-14)*/
        pos += cache[k - 20];                          /* + a(k-19)*/
        pos += cache[k - 22];                          /* + a(k-21)*/

        /* Summation of the negative coefficient terms */
        seq_t neg = cache[k - 3];                       /* - a(k-2)  */
        neg += 2 * cache[k - 8];                       /* - 2*a(k-7)*/
        neg += 3 * cache[k - 10];                      /* - 3*a(k-9)*/
        neg += cache[k - 12];                          /* - a(k-11)*/
        neg += cache[k - 17];                          /* - a(k-16)*/
        neg += 2 * cache[k - 19];                      /* - 2*a(k-18)*/
        neg += cache[k - 21];                          /* - a(k-20)*/

        cache[k - 1] = pos - neg;                      /* a(k) */
    }

    cached = n;   /* all terms up to n are now stored */
    return cache[n - 1];
}
