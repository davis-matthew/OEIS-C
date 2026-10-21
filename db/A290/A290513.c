/*
 * A290513  Number of irredundant sets in the n-ladder graph.
 * https://oeis.org/A290513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula  : a(n) = 2*a(n-1) + a(n-2) + 3*a(n-3) + 5*a(n-4)
             - 2*a(n-5) - 6*a(n-6) - 5*a(n-7) - 4*a(n-8)
             - 4*a(n-9) - 3*a(n-10) - 2*a(n-11) - 2*a(n-12)
   Initial values (offset 1):
   a(1)=3, a(2)=11, a(3)=26, a(4)=79, a(5)=224, a(6)=640,
   a(7)=1828, a(8)=5225, a(9)=14928, a(10)=42654,
   a(11)=121873, a(12)=348232 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* offset starts at 1 */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last = 0;

    /* initialisation */
    if (!cache) {
        cache_size = 13;                      /* indices 0..12 needed */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;   cache[2] = (seq_t)11;  cache[3] = (seq_t)26;
        cache[4] = (seq_t)79;  cache[5] = (seq_t)224; cache[6] = (seq_t)640;
        cache[7] = (seq_t)1828;cache[8] = (seq_t)5225;cache[9] = (seq_t)14928;
        cache[10] = (seq_t)42654;cache[11] = (seq_t)121873;
        cache[12] = (seq_t)348232;
        last = 12;
    }

    /* if the requested index is already computed */
    if (n <= last) return cache[(size_t)n];

    /* ensure the cache is large enough */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing values */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t val =
            (seq_t)2 * cache[(size_t)k-1]          + cache[(size_t)k-2]
          + (seq_t)3 * cache[(size_t)k-3]          + (seq_t)5 * cache[(size_t)k-4]
          - (seq_t)2 * cache[(size_t)k-5]          - (seq_t)6 * cache[(size_t)k-6]
          - (seq_t)5 * cache[(size_t)k-7]          - (seq_t)4 * cache[(size_t)k-8]
          - (seq_t)4 * cache[(size_t)k-9]          - (seq_t)3 * cache[(size_t)k-10]
          - (seq_t)2 * cache[(size_t)k-11]         - (seq_t)2 * cache[(size_t)k-12];

        cache[(size_t)k] = val;
    }

    last = n;
    return cache[(size_t)n];
}
