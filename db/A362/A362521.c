/*
 * A362521  Number of vertex cuts in the n-web graph.
 * https://oeis.org/A362521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  OEIS A362521: Number of vertex cuts in the n‑web graph
 *
 *  Formula from the OEIS entry:
 *        a(n) = 20*a(n-1) - 146*a(n-2) + 480*a(n-3) - 657*a(n-4)
 *              + 60*a(n-5) + 660*a(n-6) - 400*a(n-7) - 144*a(n-8)
 *              + 128*a(n-9)
 *
 *  Initial values (for n = 1 … 9):
 *        a(1)=1,  a(2)=30,  a(3)=323,  a(4)=3110,
 *        a(5)=27777, a(6)=237498, a(7)=1977439,
 *        a(8)=16202990, a(9)=131490509
 *
 *  The function memoises all computed terms so that any order of
 *  calls with arbitrary n is supported.
 */

#include <stdlib.h>

/*------------------------------------------------------------*/

seq_t a(idx_t n)
{
    /* initial values for n = 1 … 9 */
    static const seq_t init[9] = {
        (seq_t)1ULL,
        (seq_t)30ULL,
        (seq_t)323ULL,
        (seq_t)3110ULL,
        (seq_t)27777ULL,
        (seq_t)237498ULL,
        (seq_t)1977439ULL,
        (seq_t)16202990ULL,
        (seq_t)131490509ULL
    };

    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array holding a(1)…a(k) in cache[k-1] */
    static idx_t cache_len = 0;     /* number of terms currently cached */

    /* Ensure that the first 9 terms are available */
    if (cache_len < 9) {
        if (cache != NULL) {
            free(cache);
        }
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) {
            return 0;  /* allocation failure → 0 */
        }
        for (idx_t i = 0; i < 9; ++i) {
            cache[i] = init[i];
        }
    }

    /* Return a cached term if already computed */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Grow cache to size n */
    idx_t old_len = cache_len;
    cache_len = n;
    seq_t *new_cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (new_cache == NULL) {
        return 0;  /* realloc failure → 0 */
    }
    cache = new_cache;

    /* Compute terms from old_len up to n-1 (zero‑based indices) */
#if defined(__SIZEOF_INT128__)          /* use 128‑bit arithmetic when available */
    for (idx_t i = old_len; i < cache_len; ++i) {
        /* signed 128‑bit accumulator to avoid intermediate overflow */
        signed __int128 value = 0;
        value +=  20 * (signed __int128)cache[i - 1];
        value -= 146 * (signed __int128)cache[i - 2];
        value += 480 * (signed __int128)cache[i - 3];
        value -= 657 * (signed __int128)cache[i - 4];
        value +=  60 * (signed __int128)cache[i - 5];
        value += 660 * (signed __int128)cache[i - 6];
        value -= 400 * (signed __int128)cache[i - 7];
        value -= 144 * (signed __int128)cache[i - 8];
        value += 128 * (signed __int128)cache[i - 9];
        cache[i] = (seq_t)value;            /* implicit modulo by seq_t width */
    }
#else
    for (idx_t i = old_len; i < cache_len; ++i) {
        seq_t value = 0;
        value += 20 * cache[i - 1];
        value -= 146 * cache[i - 2];
        value += 480 * cache[i - 3];
        value -= 657 * cache[i - 4];
        value += 60 * cache[i - 5];
        value += 660 * cache[i - 6];
        value -= 400 * cache[i - 7];
        value -= 144 * cache[i - 8];
        value += 128 * cache[i - 9];
        cache[i] = value;
    }
#endif

    return cache[n - 1];
}
