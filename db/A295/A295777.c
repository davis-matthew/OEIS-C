/*
 * A295777  Number of nX4 0..1 arrays with each 1 adjacent to 0 or 2 king-move
 *          neighboring 1s.
 * https://oeis.org/A295777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) +6*a(n-2) +6*a(n-3) -58*a(n-4) -20*a(n-5) *
 *            +29*a(n-6) -92*a(n-7) +7*a(n-8) +2*a(n-9) -2*a(n-10), * with
 *            a(1)=8, a(2)=49, a(3)=198, a(4)=1059, a(5)=5263, * a(6)=25529,
 *            a(7)=127731, a(8)=630988, a(9)=3118368, * a(10)=15452696
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[10] = {
    (seq_t)8ULL,        (seq_t)49ULL,        (seq_t)198ULL,
    (seq_t)1059ULL,     (seq_t)5263ULL,      (seq_t)25529ULL,
    (seq_t)127731ULL,   (seq_t)630988ULL,    (seq_t)3118368ULL,
    (seq_t)15452696ULL
};

static seq_t *cache = NULL;   /* cached values a(1)...a(cache_len) */
static idx_t  cache_len = 0;  /* number of valid elements in cache    */
static size_t cache_cap = 0;  /* allocated capacity in seq_t items     */

/* ensure that cache contains at least the first n values (n>=1) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)n)
        new_cap <<= 1;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i < 10) {
            cache[i] = init[i];
        } else {
            seq_t pos = 4 * cache[i - 1] + 6 * cache[i - 2] +
                        6 * cache[i - 3] + 29 * cache[i - 6] +
                        7 * cache[i - 8] + 2 * cache[i - 9];
            seq_t neg = 58 * cache[i - 4] + 20 * cache[i - 5] +
                        92 * cache[i - 7] + 2 * cache[i -10];
            cache[i] = pos - neg;
        }
    }
    cache_len = n;
}

/* public interface required by the judge */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, return 0 as guard */
    ensure_cache(n);
    return cache[n - 1];
}
