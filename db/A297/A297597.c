/*
 * A297597  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 5 neighboring 1s.
 * https://oeis.org/A297597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Empirical recurrence for A297597
    a(n) = a(n-1)+3*a(n-2)+24*a(n-3)+23*a(n-4)-17*a(n-5)-95*a(n-6)
           -73*a(n-7)-45*a(n-8)-36*a(n-9)+4*a(n-10)-4*a(n-11)+2*a(n-12)
    Initial values (n = 1 … 12):
    1, 20, 69, 154, 752, 3145, 10131, 37929,
    150388, 546267, 2000770, 7586016
*/

#include <stdlib.h>

static const int coeff[12] = {
    1,   3,  24,  23, -17, -95, -73, -45, -36,   4,  -4,   2
};

static const unsigned long long init_raw[12] = {
    1ULL, 20ULL, 69ULL, 154ULL, 752ULL, 3145ULL,
    10131ULL, 37929ULL, 150388ULL, 546267ULL,
    2000770ULL, 7586016ULL
};

static seq_t *cache   = NULL;
static size_t  cache_size = 0;
static size_t  cache_cap  = 0;

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n<1 */

    /* extend cache up to n */
    if (cache_size < (size_t)n) {
        ensure_capacity((size_t)n);
        for (size_t i = cache_size; i < (size_t)n; ++i) {
            if (i < 12) {
                cache[i] = (seq_t)init_raw[i];
            } else {
                seq_t sum = 0;
                for (int j = 0; j < 12; ++j) {
                    seq_t term = cache[i-1-j];
                    seq_t prod  = term * (seq_t)abs(coeff[j]);
                    if (coeff[j] >= 0)
                        sum += prod;
                    else
                        sum -= prod;
                }
                cache[i] = sum;
            }
        }
        cache_size = (size_t)n;
    }
    return cache[n-1];
}
