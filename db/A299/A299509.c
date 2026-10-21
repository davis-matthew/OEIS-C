/*
 * A299509  Number of nX3 0..1 arrays with every element equal to 1, 2, 4, 5,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -7*a(n-2) +4*a(n-3) -12*a(n-4) +25*a(n-5)
 *            -22*a(n-6) +8*a(n-7) +9*a(n-8) -7*a(n-9) +13*a(n-10) -50*a(n-11)
 *            +64*a(n-12) -39*a(n-13) +10*a(n-15) (valid for n > 17). Initial
 *            values: a(1)=1, a(2)=7, a(3)=15, a(4)=29, a(5)=63, a(6)=167,
 *            a(7)=477, a(8)=1233, a(9)=3265, a(10)=8727, a(11)...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;        /* cache[i] = a(i+1) */
static size_t cache_size = 0;      /* allocated size of cache */
static size_t current_max = 0;     /* largest n with a(n) computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, undefined for n=0 */

    /* initialise cache with first 24 terms on first call */
    if (current_max == 0) {
        const size_t init_len = 24;
        if (cache_size < init_len) {
            cache = (seq_t *)realloc(cache, init_len * sizeof(seq_t));
            cache_size = init_len;
        }
        static const seq_t init_values[24] = {
            1ULL, 7ULL, 15ULL, 29ULL, 63ULL, 167ULL, 477ULL, 1233ULL,
            3265ULL, 8727ULL, 23041ULL, 61427ULL, 163443ULL,
            434073ULL, 1154961ULL, 3071109ULL, 8165955ULL,
            21718237ULL, 57753279ULL, 153583907ULL,
            408435961ULL, 1086155913ULL, 2888454053ULL,
            7681365667ULL
        };
        memcpy(cache, init_values, init_len * sizeof(seq_t));
        current_max = init_len;
    }

    /* grow cache if required */
    if (cache_size < (size_t)n) {
        cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        cache_size = (size_t)n;
    }

    /* compute missing terms using the recurrence */
    while (current_max < (size_t)n) {
        const size_t k = current_max + 1;   /* new index */
        seq_t pos = 0, neg = 0;

        /* positive contributions */
        if (k >= 2) pos += 5 * cache[k-2];      /* a(k-1)   */
        if (k >= 4) pos += 4 * cache[k-4];      /* a(k-3)   */
        if (k >= 6) pos += 25 * cache[k-6];     /* a(k-5)   */
        if (k >= 8) pos += 8  * cache[k-8];     /* a(k-7)   */
        if (k >= 9) pos += 9  * cache[k-9];     /* a(k-8)   */
        if (k >= 11) pos += 13 * cache[k-11];   /* a(k-10)  */
        if (k >= 13) pos += 64 * cache[k-13];   /* a(k-12)  */
        if (k >= 16) pos += 10 * cache[k-16];   /* a(k-15)  */

        /* negative contributions */
        if (k >= 3) neg += 7  * cache[k-3];     /* a(k-2)   */
        if (k >= 5) neg += 12 * cache[k-5];     /* a(k-4)   */
        if (k >= 7) neg += 22 * cache[k-7];     /* a(k-6)   */
        if (k >= 10) neg += 7  * cache[k-10];   /* a(k-9)   */
        if (k >= 12) neg += 50 * cache[k-12];   /* a(k-11)  */
        if (k >= 14) neg += 39 * cache[k-14];   /* a(k-13)  */

        cache[k-1] = pos - neg;
        current_max = k;
    }

    return cache[n-1];
}
