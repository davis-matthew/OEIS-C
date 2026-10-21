/*
 * A287169  Number of non-attacking king positions on a cylindrical 3 X 2n
 *          chessboard.
 * https://oeis.org/A287169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A287169: Number of non‑attacking king positions on a cylindrical
   3×2n board.
   a(0)=1, a(1)=11, a(2)=67, a(3)=503, a(4)=3939.
   For n≥5:  a(n)=11*a(n-1)-27*a(n-2)+21*a(n-3)-4*a(n-4).  */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;           /* dynamic cache for computed terms   */
    static idx_t  cache_len = 0;          /* currently allocated cache length   */
    static idx_t  last_computed = 0;      /* highest index already computed     */

    /* Ensure the cache is large enough for n+1 entries */
    if (cache_len <= n) {
        idx_t new_len = cache_len ? cache_len << 1 : 8;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                 /* unrecoverable allocation failure   */
        if (new_len > cache_len) {
            /* zero-initialize the newly allocated part */
            memset(tmp + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        }
        cache     = tmp;
        cache_len = new_len;
    }

    /* Initialise the first few terms if they have not been set yet */
    if (last_computed == 0 && n >= 0) {    /* n==0 */
        cache[0] = (seq_t)1;
        last_computed = 0;
    }
    if (last_computed < 1 && n >= 1) {
        cache[1] = (seq_t)11;
        last_computed = 1;
    }
    if (last_computed < 2 && n >= 2) {
        cache[2] = (seq_t)67;
        last_computed = 2;
    }
    if (last_computed < 3 && n >= 3) {
        cache[3] = (seq_t)503;
        last_computed = 3;
    }
    if (last_computed < 4 && n >= 4) {
        /* a(4) contains an extra constant −12 from the numerator */
        cache[4] = (seq_t)11 * cache[3]
                 - (seq_t)27 * cache[2]
                 + (seq_t)21 * cache[1]
                 - (seq_t) 4 * cache[0]
                 - (seq_t)12;
        last_computed = 4;
    }

    /* Compute remaining terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)11 * cache[i-1]
                 - (seq_t)27 * cache[i-2]
                 + (seq_t)21 * cache[i-3]
                 - (seq_t) 4 * cache[i-4];
    }
    last_computed = n;

    return cache[n];
}
