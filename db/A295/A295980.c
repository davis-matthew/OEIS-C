/*
 * A295980  Number of n X 3 0..1 arrays with each 1 adjacent to 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A295980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A295980: Number of n×3 0..1 arrays with each 1 adjacent to 2 or 3 king‑move neighboring 1s
   Recurrence (OEIS): a(n) = 4·a(n-1) – 2·a(n-2) + 21·a(n-3) – 44·a(n-4) – 12·a(n-5) – 34·a(n-6)
                     + 33·a(n-7) + 4·a(n-8) + 14·a(n-9) + a(n-10)
   Initial values (n=1..10): 1, 15, 44, 110, 581, 2354, 8452, 35474, 146560, 580023
   Computation is memoised so that a call with any index n≥1 returns the
   exact term without recomputation of previous terms.
*/

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[10] = {
    (seq_t)1,
    (seq_t)15,
    (seq_t)44,
    (seq_t)110,
    (seq_t)581,
    (seq_t)2354,
    (seq_t)8452,
    (seq_t)35474,
    (seq_t)146560,
    (seq_t)580023
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for indices below offset */

    /* Static cache */
    static seq_t *cache = NULL;          /* array of a(1)..a(cached_len) */
    static idx_t cached_len = 0;         /* number of terms populated in cache */
    static idx_t capacity   = 0;         /* allocated elements in cache */

    /* Initialise cache if first call */
    if (cache == NULL) {
        capacity = 10;
        if (capacity < n) capacity = (idx_t)(n * 2);   /* guard against very small n */
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */

        for (idx_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
        cached_len = 10;
    }

    /* Grow cache if necessary */
    if (n > capacity) {
        idx_t new_cap = capacity * 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= cached_len) {
        return cache[n-1];
    }

    /* Compute new terms up to n */
    for (idx_t i = cached_len; i < n; ++i) {
        seq_t val = 0;
        /* i corresponds to a(i+1); use cache[0] = a(1) */
        val +=  (seq_t)4  * cache[i-1];
        val -=  (seq_t)2  * cache[i-2];
        val +=  (seq_t)21 * cache[i-3];
        val -=  (seq_t)44 * cache[i-4];
        val -=  (seq_t)12 * cache[i-5];
        val -=  (seq_t)34 * cache[i-6];
        val +=  (seq_t)33 * cache[i-7];
        val +=  (seq_t)4  * cache[i-8];
        val +=  (seq_t)14 * cache[i-9];
        val +=  (seq_t)1  * cache[i-10];
        cache[i] = val;
    }
    cached_len = n;
    return cache[n-1];
}
