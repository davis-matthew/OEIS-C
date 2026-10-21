/*
 * A296552  Number of n X 6 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 3 or 6 neighboring 1s.
 * https://oeis.org/A296552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-4) + a(n-5) + 3*a(n-6) + 6*a(n-7) +
 *            2*a(n-9) + 4*a(n-10) + a(n-13); initial values: a(1)=1, a(2)=1,
 *            a(3)=5, a(4)=10, a(5)=18, a(6)=32, a(7)=62, a(8)=118, a(9)=227,
 *            a(10)=439, a(11)=839, a(12)=1592, a(13)=3037
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 69, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {            /* a(1) … a(13) */
    (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)18, (seq_t)32, (seq_t)62,
    (seq_t)118, (seq_t)227, (seq_t)439, (seq_t)839, (seq_t)1592, (seq_t)3037
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    static seq_t *cache = NULL;   /* holds a(1) … a(k) */
    static size_t  cap   = 0;     /* allocated length */
    static idx_t   last  = 0;     /* highest n for which cache is filled */

    /* allocate or enlarge cache */
    if (cache == NULL) {
        cap = (size_t) ( (n > 16) ? n : 16 );
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
    } else if ((size_t)n > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* initialise the first 13 terms if not yet done */
    if (last < 13) {
        idx_t upto = (n < 13) ? n : 13;
        for (idx_t i = last + 1; i <= upto; ++i) {
            cache[i-1] = init_vals[i-1];
        }
        last = (last < upto) ? upto : last;
    }

    /* compute terms beyond the initial 13 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 13) continue;   /* already initialised above */

        /* recurrence: indices are 1‑based in the formula;
           cache[0] holds a(1), cache[i-1] holds a(i) */
        seq_t val = cache[i-2];             /* a(i-1)   */
        val += 4 * cache[i-5];              /* 4*a(i-4) */
        val += cache[i-6];                  /* a(i-5)   */
        val += 3 * cache[i-7];              /* 3*a(i-6) */
        val += 6 * cache[i-8];              /* 6*a(i-7) */
        val += 2 * cache[i-10];             /* 2*a(i-9) */
        val += 4 * cache[i-11];             /* 4*a(i-10)*/
        val += cache[i-14];                 /* a(i-13)  */
        cache[i-1] = val;
    }
    last = (last < n) ? n : last;

    return cache[n-1];
}
