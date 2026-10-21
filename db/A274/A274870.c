/*
 * A274870  Number of set partitions of [n] into exactly six blocks such that
 *          all odd elements are in blocks with an odd index and all even
 *          elements are in blocks with an even index.
 * https://oeis.org/A274870
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 24, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274870, offset 6.
   Recurrence derived from the generating function
   G(x) = x^6 / ((1‑x)(1‑3x)(2x+1)(2x‑1)(x+1)(6x²‑1)(2x²‑1)):
   a[n] = 3·a[n‑1] +13·a[n‑2] –39·a[n‑3] –56·a[n‑4] +168·a[n‑5]
          + 92·a[n‑6] –276·a[n‑7] –48·a[n‑8] +144·a[n‑9] + (n==6?1:0)
   for all n ≥ 6, with a[0]…a[5] = 0. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* defined only for n ≥ 6; for smaller indices return 0 */
    if (n < 6) return 0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of valid entries in cache   */

    /* initialise cache with indices 0…5 set to 0 */
    if (cache_size == 0) {
        cache_size = 6;
        cache = (seq_t*)calloc((size_t)cache_size, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    idx_t old_size = cache_size;
    /* extend cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        /* zero‑initialise newly allocated part */
        memset((void*)(cache + old_size), 0,
               (size_t)(new_size - old_size) * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute missing terms from old_size up to n */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t const_term = (i == 6) ? (seq_t)1 : (seq_t)0;

        seq_t pos = 0;
        seq_t neg = 0;

        if (i >= 1) pos += (seq_t)3   * cache[i - 1];
        if (i >= 2) pos += (seq_t)13  * cache[i - 2];
        if (i >= 5) pos += (seq_t)168 * cache[i - 5];
        if (i >= 6) pos += (seq_t)92  * cache[i - 6];
        if (i >= 9) pos += (seq_t)144 * cache[i - 9];

        if (i >= 3) neg += (seq_t)39  * cache[i - 3];
        if (i >= 4) neg += (seq_t)56  * cache[i - 4];
        if (i >= 7) neg += (seq_t)276 * cache[i - 7];
        if (i >= 8) neg += (seq_t)48  * cache[i - 8];

        seq_t val = pos;
        val -= neg;
        val += const_term;
        cache[i] = val;
    }

    return cache[n];
}
