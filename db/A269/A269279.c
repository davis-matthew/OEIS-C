/*
 * A269279  Number of 4Xn 0..3 arrays with some element plus some horizontally
 *          or antidiagonally adjacent neighbor totalling three exactly once.
 * https://oeis.org/A269279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A269279: Number of 4×n 0..3 arrays with exactly one horizontal or
   anti‑diagonal neighbour pair summing to 3.
   Recurrence (order 8) for n > 12:
   a(n) =  96 a(n−1) − 3108 a(n−2) + 40720 a(n−3) − 265326 a(n−4)
           + 931200 a(n−5) − 1766452 a(n−6) + 1678992 a(n−7) − 622521 a(n−8)
   Initial terms (n=1…13):
   0, 20412, 1365336, 74853576, 3719884392, 174924572760,
   7934992835112, 350946381867480, 15232155757251048,
   651580499598523992, 27551808205246504872,
   1154085846201751972824, 47965168186749101620584
*/
#include <stdlib.h>

/* Forward declarations of utilities that may provide arbitrary‑precision
   support. They are available whenever the build uses an arbitrary‑precision
   seq_t and are harmless if seq_t is a plain integer type. */
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;   /* array a(1)..a(size) */
    static size_t cap      = 0;      /* allocated capacity (number of seq_t slots) */
    static size_t size     = 0;      /* number of terms stored */

    /* initial terms – strings so they fit any seq_t */
    const idx_t init_len   = 13;
    const char *init_strs[13] = {
        "0",
        "20412",
        "1365336",
        "74853576",
        "3719884392",
        "174924572760",
        "7934992835112",
        "350946381867480",
        "15232155757251048",
        "651580499598523992",
        "27551808205246504872",
        "1154085846201751972824",
        "47965168186749101620584"
    };

    /* first call – initialise cache */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)init_len * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory – give up */
        for (idx_t i = 0; i < init_len; ++i)
            cache[i] = seq_from_str(init_strs[i]);   /* works for integer or big‑int */
        cap  = init_len;
        size = init_len;
    }

    if (n == 0) return 0;                  /* undefined for n < 1 */

    /* grow cache if necessary */
    if ((size_t)size < n) {
        size_t new_cap = n;
        if (new_cap < cap * 2) new_cap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* out of memory */
        cache = tmp;
        cap   = new_cap;

        /* compute new terms using the recurrence, using seq_t arithmetic */
        for (idx_t idx = (idx_t)size; idx < n; ++idx) { /* idx is 0‑based for a(idx+1) */
            seq_t val = 96 * cache[idx - 1];
            val -= 3108 * cache[idx - 2];
            val += 40720 * cache[idx - 3];
            val -= 265326 * cache[idx - 4];
            val += 931200 * cache[idx - 5];
            val -= 1766452 * cache[idx - 6];
            val += 1678992 * cache[idx - 7];
            val -= 622521 * cache[idx - 8];
            cache[idx] = val;
        }
        size = n;
    }

    return cache[n - 1];
}
