/*
 * A280551  Number of n X 5 0..1 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors, with the exception of
 *          exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -12*a(n-2) +5*a(n-3) +12*a(n-4) -12*a(n-5)
 *            -3*a(n-6) +6*a(n-7) -a(n-9) for n>12 with initial values:
 *            a(1)=2, a(2)=87, a(3)=325, a(4)=832, a(5)=1764, a(6)=3438,
 *            a(7)=6386, a(8)=11506, a(9)=20389, a(10)=35757, a(11)=62317,
 *            a(12)=108150
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 31, u64 n <= 75, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[13] = {
    0ULL,    // a(0) unused
    2ULL,    // a(1)
    87ULL,   // a(2)
    325ULL,  // a(3)
    832ULL,  // a(4)
    1764ULL, // a(5)
    3438ULL, // a(6)
    6386ULL, // a(7)
    11506ULL,// a(8)
    20389ULL,// a(9)
    35757ULL,// a(10)
    62317ULL,// a(11)
    108150ULL// a(12)
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* a(0) is not defined, return 0 */

    /* static cache for memoization */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  computed = 0;   /* highest index computed so far */

    /* grow cache if necessary */
    if (cache_cap <= n) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialize base values once */
    if (computed == 0) {
        for (idx_t i = 1; i < 13; ++i) cache[i] = base_vals[i];
        computed = 12;
    }

    /* already computed? */
    if (n <= computed) return cache[n];

    /* compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t pos = (seq_t)6 * cache[i-1];
        pos += (seq_t)5 * cache[i-3];
        pos += (seq_t)12 * cache[i-4];
        pos += (seq_t)6 * cache[i-7];

        seq_t neg = (seq_t)12 * cache[i-2];
        neg += (seq_t)12 * cache[i-5];
        neg += (seq_t)3 * cache[i-6];
        neg += cache[i-9];

        cache[i] = pos - neg;   /* safe: pos >= neg for all n */
    }
    computed = n;
    return cache[n];
}
