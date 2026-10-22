/*
 * A351123  Irregular triangle read by rows: row n lists the partial sums of
 *          the number of divisions by 2 after each tripling step in the
 *          Collatz trajectory of 2n+1.
 * https://oeis.org/A351123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A351123: Irregular triangle read by rows.  
   For row n (n ≥ 1) let m = 2n+1.  We iterate the Collatz
   procedure only on odd numbers: on each odd x we set  
       t = 3*x + 1;  k = v₂(t)   (number of trailing 2‑factors of t)
   The row consists of the partial sums of the sequence k, k, k, ...  
   Up to the last odd < 1.  a(n) is the n‑th element of this
   triangle read row by row (first row is T(1,0),T(1,1), ...).
   The function below keeps a cache of already computed terms so
   that arbitrary access works in O(1) amortised time and is
   independent of the order of calls.
   It works for any unsigned integer type seq_t given by the build.
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed a(n) values   */
static size_t cache_len = 0;         /* number of values stored         */
static size_t cache_cap = 0;         /* current capacity of the array   */
static idx_t last_row = 0;           /* last processed row number       */

/* Count trailing zero bits of an arbitrary unsigned integer */
static unsigned int trailing_zeros(seq_t x) {
    unsigned int c = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    /* return cached result if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* grow the cache until it contains at least n terms */
    while (cache_len < n) {
        /* process next row */
        ++last_row;
        seq_t cur = (seq_t)2 * last_row + (seq_t)1;  /* odd number 2n+1 */
        seq_t sum = (seq_t)0;

        /* iterate Collatz on odd numbers only */
        while (cur != (seq_t)1) {
            seq_t t = cur * (seq_t)3 + (seq_t)1;
            unsigned int k = trailing_zeros(t);
            sum += (seq_t)k;

            /* append the new partial sum to the cache */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sum;

            /* move to the next odd number in the trajectory */
            cur = t >> k;
        }
    }
    return cache[n - 1];
}
