/*
 * A250734  Number of (5+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 4*a(n-1) -
 *            17*a(n-3) + 17*a(n-4) + 12*a(n-5) * - 23*a(n-6) + 6*a(n-7) +
 *            3*a(n-8) - a(n-9) (n > 12) * with initial values a(1)=99,
 *            a(2)=230, a(3)=600, a(4)=1500, a(5)=3605, * a(6)=8514,
 *            a(7)=19887, a(8)=46315, a(9)=107565, a(10)=249853, *
 *            a(11)=579962,...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 47, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* First 12 terms of the sequence */
static const seq_t init_vals[12] = {
    (seq_t)99,  (seq_t)230,  (seq_t)600,  (seq_t)1500,
    (seq_t)3605, (seq_t)8514, (seq_t)19887, (seq_t)46315,
    (seq_t)107565, (seq_t)249853, (seq_t)579962, (seq_t)1346953
};

/* Cache for memoisation */
static idx_t cache_max = 0;   /* largest n computed so far */
static seq_t *cache = NULL;   /* indices 1 .. cache_max inclusive  */
static size_t cache_cap = 0;  /* capacity of cache array (number of slots) */

/* Helper to reallocate the cache array to hold at least 'need' elements. */
static void ensure_cache_size(idx_t need)
{
    if (need <= cache_cap) return;                 /* already large enough */
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)need) new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t)); /* C/C++ safe */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below offset irrelevant */

    /* Expand cache if necessary */
    if (n > cache_max) {
        ensure_cache_size(n + 1);   /* indices 0 .. n inclusive */

        /* Compute missing terms */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 12) {
                cache[i] = init_vals[i - 1];
            } else {
                seq_t pos = 0, neg = 0;
                pos += (seq_t)4 * cache[i - 1];
                pos += (seq_t)17 * cache[i - 4];
                pos += (seq_t)12 * cache[i - 5];
                pos += (seq_t)6  * cache[i - 7];
                pos += (seq_t)3  * cache[i - 8];

                neg += (seq_t)17 * cache[i - 3];
                neg += (seq_t)23 * cache[i - 6];
                neg += (seq_t)1  * cache[i - 9];

                cache[i] = pos - neg;
            }
        }
        cache_max = n;
    }
    return cache[n];
}
