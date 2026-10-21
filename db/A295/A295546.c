/*
 * A295546  Number of n X 3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1, 3 or 4 1's.
 * https://oeis.org/A295546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) – 6*a(n-2) + 2*a(n-3) – 3*a(n-4) + 12*a(n-5) –
 *            24*a(n-6) + a(n-7) + 17*a(n-8) – 3*a(n-9) + 6*a(n-10) +
 *            8*a(n-11) + 5*a(n-12) – 15*a(n-13) + 12*a(n-14) – 9*a(n-15) +
 *            2*a(n-16) – a(n-17) * Initial values: a(1)=3, a(2)=11, a(3)=34,
 *            a(4)=116, a(5)=391, * a(6)=1304, a(7)=4374, a(8)=146...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[18] = {
        (seq_t)0,          /* 0 unused */
        (seq_t)3, (seq_t)11, (seq_t)34, (seq_t)116,
        (seq_t)391, (seq_t)1304, (seq_t)4374, (seq_t)14645,
        (seq_t)49051, (seq_t)164353, (seq_t)550580,
        (seq_t)1844483, (seq_t)6179157, (seq_t)20700471,
        (seq_t)69348050, (seq_t)232320799, (seq_t)778290532
    };

    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static idx_t  computed_up_to = 0;

    if (n == 0) return (seq_t)0;          /* offset is 1, but guard for n==0 */

    /* Allocate or enlarge the cache to at least n (or 18 for the first run) */
    if (cache == NULL) {
        idx_t alloc_len = (n > 17) ? n : 18;
        cache = (seq_t*)malloc((alloc_len + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed – unlikely in tests */
        cache_len = alloc_len;
        /* copy initial values 1..17 */
        for (idx_t k = 1; k <= 17 && k <= cache_len; ++k) cache[k] = init[k];
        computed_up_to = (cache_len >= 17) ? 17 : cache_len;
    } else if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t*)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* reallocation failed – unlikely */
        cache = tmp;
        cache_len = new_len;
        /* no need to initialize new elements – they will be filled by recurrence */
    }

    /* If the requested term is already computed, return it */
    if (n <= computed_up_to) return cache[n];

    /* Compute terms from the next missing index up to n */
    idx_t start = (computed_up_to >= 17) ? computed_up_to + 1 : 18;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] =
            (seq_t)5   * cache[i-1]  - (seq_t)6   * cache[i-2]
          + (seq_t)2   * cache[i-3]  - (seq_t)3   * cache[i-4]
          + (seq_t)12  * cache[i-5]  - (seq_t)24  * cache[i-6]
          + (seq_t)1   * cache[i-7]  + (seq_t)17  * cache[i-8]
          - (seq_t)3   * cache[i-9]  + (seq_t)6   * cache[i-10]
          + (seq_t)8   * cache[i-11] + (seq_t)5   * cache[i-12]
          - (seq_t)15  * cache[i-13] + (seq_t)12  * cache[i-14]
          - (seq_t)9   * cache[i-15] + (seq_t)2   * cache[i-16]
          - (seq_t)1   * cache[i-17];
    }

    computed_up_to = n;
    return cache[n];
}
