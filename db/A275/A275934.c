/*
 * A275934  Shifts 4 places left under binomial transform.
 * https://oeis.org/A275934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 17, u32 n <= 26, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275934
   Recurrence:  a(0)=0, a(1)=0, a(2)=0, a(3)=1
   For n≥4  :  a(n) = Σ_{i=0}^{n-4} binomial(n-4,i) * a(i)
   (i.e.  a(n+4) = Σ_{i=0}^{n} binomial(n,i) * a(i) )
   This implementation memoises the sequence in a static cache.
   The cache grows as needed; no external symbols other than the
   required function definition are used.  All arithmetic is done in
   seq_t.  This code compiles under C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of initialised elements   */
    static size_t cache_cap  = 0;   /* capacity of the allocated array  */

    /* ensure enough capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;        /* grow faster if needed   */
        if (new_cap == 0) new_cap = 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) exit(1);
        if (new_cap > cache_cap) cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t k = cached_len; k <= n; ++k) {
        seq_t val;
        if (k < 4) {
            val = (k == 3) ? (seq_t)1 : (seq_t)0;
        } else {
            idx_t m = k - 4;                 /* number of terms to sum */
            seq_t sum   = 0;
            seq_t binom = 1;                 /* binomial(m,0) */
            sum += binom * cache[0];         /* i = 0 */

            for (idx_t i = 1; i <= m; ++i) {
                /* binom = binom * (m-i+1) / i  */
                binom = binom * (seq_t)(m - i + 1) / (seq_t)i;
                sum += binom * cache[i];
            }
            val = sum;
        }
        cache[k] = val;
    }

    cached_len = n + 1;    /* all values up to n are now cached */
    return cache[n];
}
