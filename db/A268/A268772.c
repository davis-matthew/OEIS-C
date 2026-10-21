/*
 * A268772  Number of nX6 0..2 arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two exactly once.
 * https://oeis.org/A268772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A268772
   a(n) = 6*a(n-1) +51*a(n-2) -214*a(n-3) -1074*a(n-4) +2018*a(n-5) +7713*a(n-6)
          -10572*a(n-7) -22926*a(n-8) +30116*a(n-9) +25283*a(n-10) -32400*a(n-11)
          -15148*a(n-12) +15184*a(n-13) +5660*a(n-14) -2688*a(n-15) -1024*a(n-16)
   Initial values: a(1)=240, a(2)=620, a(3)=5214, a(4)=46312, a(5)=387146,
   a(6)=3104544, a(7)=24222418, a(8)=185142872, a(9)=1393319226,
   a(10)=10357051740, a(11)=76224579034, a(12)=556383657268,
   a(13)=4033179662378, a(14)=29064236056520, a(15)=208382539816438,
   a(16)=1487439977791192, a(17)=10576114792480666, a(18)=74940142727337224
 */

#include <stdlib.h>

/* static lookup table for the first 18 terms */
static const seq_t init_vals[18] = {
    (seq_t)240ULL,
    (seq_t)620ULL,
    (seq_t)5214ULL,
    (seq_t)46312ULL,
    (seq_t)387146ULL,
    (seq_t)3104544ULL,
    (seq_t)24222418ULL,
    (seq_t)185142872ULL,
    (seq_t)1393319226ULL,
    (seq_t)10357051740ULL,
    (seq_t)76224579034ULL,
    (seq_t)556383657268ULL,
    (seq_t)4033179662378ULL,
    (seq_t)29064236056520ULL,
    (seq_t)208382539816438ULL,
    (seq_t)1487439977791192ULL,
    (seq_t)10576114792480666ULL,
    (seq_t)74940142727337224ULL
};

/* Computes the n-th term of A268772 */
seq_t a(idx_t n)
{
    /* undefined for indices < 1; return 0 to keep behaviour trivial */
    if (n < 1)
        return 0;

    /* use a cache that grows on demand */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored in cache */

    /* initialise on first call */
    if (!cache) {
        cache_len = 18;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init_vals[i];
    }

    /* return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* extend cache to cover index n */
    size_t needed = (size_t)n;
    seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) return 0;               /* allocation failure */
    cache = tmp;

    /* compute missing terms */
    for (size_t k = cache_len + 1; k <= needed; ++k) {   /* k is 1‑based index */
        seq_t pos = 0, neg = 0;
        /* positive coefficients */
        pos += (seq_t)6     * cache[k - 2];
        pos += (seq_t)51    * cache[k - 3];
        pos += (seq_t)2018  * cache[k - 6];
        pos += (seq_t)7713  * cache[k - 7];
        pos += (seq_t)30116 * cache[k -10];
        pos += (seq_t)25283 * cache[k -11];
        pos += (seq_t)15184 * cache[k -14];
        pos += (seq_t)5660  * cache[k -15];
        /* negative coefficients */
        neg += (seq_t)214  * cache[k - 4];
        neg += (seq_t)1074 * cache[k - 5];
        neg += (seq_t)10572* cache[k - 8];
        neg += (seq_t)22926* cache[k - 9];
        neg += (seq_t)32400* cache[k -12];
        neg += (seq_t)15148* cache[k -13];
        neg += (seq_t)2688 * cache[k -16];
        neg += (seq_t)1024 * cache[k -17];
        cache[k - 1] = pos - neg;
    }

    cache_len = needed;
    return cache[n - 1];
}
