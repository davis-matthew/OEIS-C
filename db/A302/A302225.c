/*
 * A302225  Number of 2Xn 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(1)=0, a(2)=3, a(3)=14, a(4)=45, a(5)=146,
 *            a(6)=537, a(n) = 2*a(n-1)+3*a(n-2)+6*a(n-3)+10*a(n-4)+4*a(n-5)
 *            (n>6)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* cache of computed terms */
static const seq_t base[6] = {0, 3, 14, 45, 146, 537};

#ifdef __cplusplus
static std::vector<seq_t> cache;
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* n < offset is undefined */
#ifdef __cplusplus
    /* grow cache if necessary */
    if (cache.size() < (size_t)n) {
        size_t prev = cache.size();
        cache.resize(n);
        for (idx_t i = prev; i < n; ++i) {
            if (i < 6) {
                cache[i] = base[i];
            } else {
                cache[i] = ((seq_t)2) * cache[i-1] +
                           ((seq_t)3) * cache[i-2] +
                           ((seq_t)6) * cache[i-3] +
                           ((seq_t)10) * cache[i-4] +
                           ((seq_t)4) * cache[i-5];
            }
        }
    }
    return cache[n-1];
#else
    /* C implementation uses a raw pointer and realloc */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    if (cache_size < (size_t)n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) abort();                 /* allocation failed */
        cache = new_cache;
        for (size_t i = cache_size; i < (size_t)n; ++i) {
            if (i < 6) {
                cache[i] = base[i];
            } else {
                cache[i] = ((seq_t)2) * cache[i-1] +
                           ((seq_t)3) * cache[i-2] +
                           ((seq_t)6) * cache[i-3] +
                           ((seq_t)10) * cache[i-4] +
                           ((seq_t)4) * cache[i-5];
            }
        }
        cache_size = n;
    }
    return cache[n-1];
#endif
}
