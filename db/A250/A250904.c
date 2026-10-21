/*
 * A250904  Number of (6+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 14, u64 n <= 35, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250904: Number of (6+1)×(n+1) 0..2 arrays with the given monotonicity constraints
   Empirical linear recurrence of order 9 (valid for n ≥ 10):
       a(n) = 14 a(n‑1) – 85 a(n‑2) + 294 a(n‑3) – 639 a(n‑4)
             + 906 a(n‑5) – 839 a(n‑6) + 490 a(n‑7) – 164 a(n‑8)
             + 24 a(n‑9)
   Initial values:
       a(1)=13111, a(2)=47061, a(3)=136509, a(4)=357335,
       a(5)=890199, a(6)=2168405, a(7)=5232461,
       a(8)=12579759, a(9)=30207983. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n = 0 */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initial values (index 0 unused) */
    static const seq_t init[10] = {
        0,          /* dummy for index 0 */
        13111,      /* a(1) */
        47061,      /* a(2) */
        136509,     /* a(3) */
        357335,     /* a(4) */
        890199,     /* a(5) */
        2168405,    /* a(6) */
        5232461,    /* a(7) */
        12579759,   /* a(8) */
        30207983    /* a(9) */
    };

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 9;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        if (cache == NULL)         /* allocation failure – give up */
            return 0;
        for (idx_t i = 0; i <= cache_len; ++i)
            cache[i] = init[i];
    }

    /* Return already computed value if available */
    if (n <= cache_len)
        return cache[n];

    /* Grow cache to accommodate up to n */
    idx_t new_len = n;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (new_cache == NULL)       /* keep old cache on failure */
        new_cache = cache;
    cache = new_cache;

    /* Compute missing terms via recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        val += (seq_t)14   * cache[i - 1];
        val -= (seq_t)85   * cache[i - 2];
        val += (seq_t)294  * cache[i - 3];
        val -= (seq_t)639  * cache[i - 4];
        val += (seq_t)906  * cache[i - 5];
        val -= (seq_t)839  * cache[i - 6];
        val += (seq_t)490  * cache[i - 7];
        val -= (seq_t)164  * cache[i - 8];
        val += (seq_t)24   * cache[i - 9];
        cache[i] = val;
    }

    cache_len = n;                 /* update maximum computed index */
    return cache[n];
}
