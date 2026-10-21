/*
 * A287485  Number of independent vertex sets and vertex covers in the n-web
 *          graph.
 * https://oeis.org/A287485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2) + 4*a(n-3) Initial values: a(1)=2,
 *            a(2)=20, a(3)=68
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <type_traits>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* largest index computed so far */

    if (n < 1) return (seq_t)0;           /* undefined for n=0 */

    /* Return memoised value if already computed */
    if (n <= cache_size) return cache[n];

    /* Grow the cache to hold indices up to n (index 0 unused) */
    idx_t new_size = n + 1;                /* allocate one extra for 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                    /* allocation failed */
    cache = tmp;

    /* Compute missing values */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        if (k == 1) {
            cache[k] = (seq_t)2;
        } else if (k == 2) {
            cache[k] = (seq_t)20;
        } else if (k == 3) {
            cache[k] = (seq_t)68;
        } else {
#ifdef __cplusplus
            /* C++ build: use a wider intermediate type for integral seq_t
               to avoid overflow, otherwise use big‑integer arithmetic. */
            if constexpr (std::is_integral<seq_t>::value) {
                using Wider =
                    std::conditional_t< (sizeof(seq_t) <= 4),
                        unsigned long long,
                        unsigned __int128>;
                Wider v1 = (Wider)cache[k-1] * (Wider)2;
                Wider v2 = (Wider)cache[k-2] * (Wider)8;
                Wider v3 = (Wider)cache[k-3] * (Wider)4;
                cache[k] = (seq_t)(v1 + v2 + v3);
            } else
#endif
            {
                /* C build or big‑integer build: use ordinary operators. */
                cache[k] = (seq_t)2 * cache[k-1] +
                           (seq_t)8 * cache[k-2] +
                           (seq_t)4 * cache[k-3];
            }
        }
    }
    cache_size = n;
    return cache[n];
}
