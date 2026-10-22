/*
 * A396312  Number of tilings of an (h+3) X n rectangle by unit squares and
 *          the Ferrers tile (h,2,2,1), for h >= 4.
 * https://oeis.org/A396312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=1; for n≥4,
 *            a(n)=a(n-1)+4*a(n-4)+3*a(n-5)+3*a(n-6)+6*a(n-7)+2*a(n-8)+2*a(n-9)+4*a(n-10)+a(n-13)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* memoised DP cache */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;

    /* If already computed, return */
    if (n < cache_sz) return cache[n];

    /*  Base values required by the recurrence */
    static const seq_t base_vals[13] = {
        (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)1,
        (seq_t)5,  (seq_t)12, (seq_t)22, (seq_t)38,
        (seq_t)72, (seq_t)148, (seq_t)301, (seq_t)593,
        (seq_t)1149
    };
    const size_t base_cnt = sizeof(base_vals)/sizeof(base_vals[0]);

    /* Resize cache to hold up to n */
    size_t new_sz = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure – unlikely for n ≤ 10000 */
    cache = tmp;

    /* Compute new entries */
    for (idx_t i = cache_sz; i <= n; ++i)
    {
        if (i < (idx_t)base_cnt)
            cache[i] = base_vals[i];
        else
        {
            seq_t val = cache[i-1];
            val += cache[i-4] * (seq_t)4;
            val += cache[i-5] * (seq_t)3;
            val += cache[i-6] * (seq_t)3;
            val += cache[i-7] * (seq_t)6;
            val += cache[i-8] * (seq_t)2;
            val += cache[i-9] * (seq_t)2;
            val += cache[i-10] * (seq_t)4;
            val += cache[i-13];
            cache[i] = val;
        }
    }
    cache_sz = new_sz;
    return cache[n];
}
