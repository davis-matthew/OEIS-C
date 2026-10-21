/*
 * A264005  Number of (2+1)X(n+1) arrays of permutations of 0..n*3+2 with each
 *          element having index change (+-,+-) 0,0 1,2 or 1,0.
 * https://oeis.org/A264005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence (valid for n ≥ 17) a(n) = 9*a(n-1) -
 *            62*a(n-3) + 108*a(n-4) - 456*a(n-5) - 800*a(n-6) + 9808*a(n-7) -
 *            8032*a(n-8) - 256*a(n-9) + 12800*a(n-10) - 20992*a(n-11) +
 *            27648*a(n-12) - 24576*a(n-13) + 32768*a(n-15) - 65536*a(n-16)
 *            Initial terms (a(1)…a(16)): 9, 99, 1089, 8679,...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 16 initial terms, casted so truncation works for small seq_t types   */
static const seq_t init[17] = {
    0ULL,          /* unused index 0 */
    9ULL,
    99ULL,
    1089ULL,
    8679ULL,
    69169ULL,
    568343ULL,
    4669921ULL,
    38152455ULL,
    311699025ULL,
    2548552215ULL,
    20837788609ULL,
    170360646951ULL,
    1392794147889ULL,
    11387102116407ULL,
    93097816935841ULL,
    761140735852103ULL
};

/* Cache for computed values */
static seq_t *cache = NULL;          /* pointer to cache array */
static idx_t  cache_size = 0;        /* allocated size (in elements)     */
static idx_t  last_computed = 0;     /* largest n that has been computed  */

/* Ensure the cache can hold at least needed+1 elements (index 0 unused) */
static void ensure_capacity(idx_t needed)
{
    if (needed < cache_size)
        return;
    idx_t new_size = cache_size ? cache_size : 17;
    while (new_size <= needed)
        new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!tmp) abort();          /* terminate on allocation failure */
    cache = tmp;
    cache_size = new_size;
}

/* Entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* outside offset, return 0 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 17;          /* enough for a(1) … a(16) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();
        for (idx_t i = 1; i <= (idx_t)16; ++i)
            cache[i] = init[i];
        last_computed = 16;
    }

    /* Compute new values up to n as needed */
    if (n > last_computed) {
        ensure_capacity(n);
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i <= (idx_t)16) {
                cache[i] = init[i];
            } else {
                seq_t sum = 0;
                sum += (seq_t)9    * cache[i - 1];
                sum -= (seq_t)62   * cache[i - 3];
                sum += (seq_t)108  * cache[i - 4];
                sum -= (seq_t)456  * cache[i - 5];
                sum -= (seq_t)800  * cache[i - 6];
                sum += (seq_t)9808 * cache[i - 7];
                sum -= (seq_t)8032 * cache[i - 8];
                sum -= (seq_t)256  * cache[i - 9];
                sum += (seq_t)12800 * cache[i -10];
                sum -= (seq_t)20992 * cache[i -11];
                sum += (seq_t)27648 * cache[i -12];
                sum -= (seq_t)24576 * cache[i -13];
                sum += (seq_t)32768 * cache[i -15];
                sum -= (seq_t)65536 * cache[i -16];
                cache[i] = sum;
            }
        }
        last_computed = n;
    }

    return cache[n];
}
