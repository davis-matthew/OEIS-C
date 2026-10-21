/*
 * A249984  Number of length 4+1 0..2*n arrays with the sum of the absolute
 *          values of adjacent differences equal to 4*n.
 * https://oeis.org/A249984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)
 *            a(1)=64 , a(2)=384 , a(3)=1242 , a(4)=3030 , a(5)=6252
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 173, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Note:  cache_len = highest n whose a(n) is stored.
    cache_cap = allocated size (in elements); indices 0..cache_cap-1 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache        = NULL;   /* 1‑based array of a(n) values   */
static size_t cache_cap    = 0;      /* allocated capacity (elements) */
static idx_t  cache_len    = 0;      /* largest index computed so far  */

/*  Ensure that cache has space for indices 0..n   */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);   /* because we use 0..n inclusive    */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/*  Return A249984(n)  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<=0 */

    ensure_cache(n);

    /*  Initialise the first five terms once  */
    if (cache_len < 5) {
        cache[1] = (seq_t)64;
        cache[2] = (seq_t)384;
        cache[3] = (seq_t)1242;
        cache[4] = (seq_t)3030;
        cache[5] = (seq_t)6252;
        cache_len = 5;
    }

    /*  Already known  */
    if (n <= cache_len) return cache[n];

    /*  Compute new terms up to n  */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = (seq_t)5   * cache[i - 1]
                 - (seq_t)10  * cache[i - 2]
                 + (seq_t)10  * cache[i - 3]
                 - (seq_t)5   * cache[i - 4]
                 +           cache[i - 5];
    }
    cache_len = n;
    return cache[n];
}
