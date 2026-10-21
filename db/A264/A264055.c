/*
 * A264055  Number of (n+1) X (3+1) arrays of permutations of 0..n*4+3 with
 *          each element having index change +-(.,.) 0,0 1,2 or 2,-2.
 * https://oeis.org/A264055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264055  
   Empirical 17‑term linear recurrence, initial values shown. */
#include <stdlib.h>

static seq_t *cache      = NULL;     /* array of computed values (1‑based)   */
static idx_t max_comp    = 0;        /* highest index currently computed    */
static size_t cache_cap  = 0;        /* allocated capacity (elements, index 0 unused) */

/* Ensure that cache contains values up to index n (1‑based).  */
static void ensure_cache(idx_t n)
{
    if (n <= max_comp)
        return;

    /* Allocate enough space if necessary. */
    size_t needed = (size_t)(n + 1);
    if (cache == NULL) {
        if (needed < 18u)
            needed = 18u;
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = needed;
    } else if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialise first 17 terms exactly once. */
    if (max_comp == 0) {
        cache[0] = (seq_t)0;
        cache[1]  = (seq_t)4;
        cache[2]  = (seq_t)64;
        cache[3]  = (seq_t)324;
        cache[4]  = (seq_t)2025;
        cache[5]  = (seq_t)15625;
        cache[6]  = (seq_t)102400;
        cache[7]  = (seq_t)692224;
        cache[8]  = (seq_t)4826809;
        cache[9]  = (seq_t)32867289;
        cache[10] = (seq_t)224820036;
        cache[11] = (seq_t)1544804416;
        cache[12] = (seq_t)10578122500;
        cache[13] = (seq_t)72482100625;
        cache[14] = (seq_t)496981290961;
        cache[15] = (seq_t)3405885014016;
        cache[16] = (seq_t)23343276294144;
        cache[17] = (seq_t)160005726539569;
        max_comp = 17;
    }

    /* Compute any new values using the empirical recurrence. */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        seq_t v = 0;
        v += (seq_t)7   * cache[k - 1];
        v -=          cache[k - 2];
        v += (seq_t)55  * cache[k - 3];
        v -= (seq_t)385 * cache[k - 4];
        v += (seq_t)55  * cache[k - 5];
        v -= (seq_t)385 * cache[k - 6];
        v += (seq_t)2695 * cache[k - 7];
        v -= (seq_t)385 * cache[k - 8];
        v += (seq_t)385 * cache[k - 9];
        v -= (seq_t)2695 * cache[k -10];
        v += (seq_t)385 * cache[k -11];
        v -= (seq_t)55  * cache[k -12];
        v += (seq_t)385 * cache[k -13];
        v -= (seq_t)55  * cache[k -14];
        v +=          cache[k -15];
        v -= (seq_t)7   * cache[k -16];
        v +=          cache[k -17];
        cache[k] = v;
    }
    if (n > max_comp)
        max_comp = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 */
    ensure_cache(n);
    return cache[n];
}
