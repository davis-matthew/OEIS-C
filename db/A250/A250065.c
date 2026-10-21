/*
 * A250065  Number of length 6+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining five terms.
 * https://oeis.org/A250065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A250065
   Recurrence (n > 13):
   a(n) = 13*a(n-1) - 78*a(n-2) + 286*a(n-3) - 715*a(n-4) + 1287*a(n-5)
          - 1716*a(n-6) + 1716*a(n-7) - 1287*a(n-8) + 715*a(n-9)
          - 286*a(n-10) + 78*a(n-11) - 13*a(n-12) + a(n-13)
   Initial values:
   a(1)=21, a(2)=12284, a(3)=915064, a(4)=22838540,
   a(5)=295152625, a(6)=2461761504, a(7)=15032948568,
   a(8)=72725094312, a(9)=293743941525, a(10)=1027603529940,
   a(11)=3197464381472, a(12)=9027047995732, a(13)=23478340317185.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Initial 13 terms of the sequence */
static const seq_t init_vals[13] = {
    (seq_t)21ULL,
    (seq_t)12284ULL,
    (seq_t)915064ULL,
    (seq_t)22838540ULL,
    (seq_t)295152625ULL,
    (seq_t)2461761504ULL,
    (seq_t)15032948568ULL,
    (seq_t)72725094312ULL,
    (seq_t)293743941525ULL,
    (seq_t)1027603529940ULL,
    (seq_t)3197464381472ULL,
    (seq_t)9027047995732ULL,
    (seq_t)23478340317185ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* not used, but safe */

    size_t idx = (size_t)n;                     /* 1‑based index */

    /* static cache of computed values */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;                 /* number of entries filled (1‑based) */

    /* Ensure enough capacity */
    if (cache_cap < idx) {
        size_t new_cap = (idx > 20 ? idx : 20);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                              /* allocation failed – give up */
            /* In a real implementation we would handle this more gracefully */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first 13 terms if not already done */
    if (cache_len < 13) {
        size_t start = cache_len;
        for (size_t i = start; i < 13 && i < cache_cap; ++i)
            cache[i] = init_vals[i];
        cache_len = (cache_len < 13 ? 13 : cache_len);
    }

    /* Compute remaining terms up to n */
    while (cache_len < idx) {
        size_t i = cache_len;                    /* 0‑based index of a(i+1) */
        /* i >= 13 because cache_len >= 13 after preceding block */
        seq_t v = (seq_t)13   * cache[i-1]
                - (seq_t)78   * cache[i-2]
                + (seq_t)286  * cache[i-3]
                - (seq_t)715  * cache[i-4]
                + (seq_t)1287 * cache[i-5]
                - (seq_t)1716 * cache[i-6]
                + (seq_t)1716 * cache[i-7]
                - (seq_t)1287 * cache[i-8]
                + (seq_t)715  * cache[i-9]
                - (seq_t)286  * cache[i-10]
                + (seq_t)78   * cache[i-11]
                - (seq_t)13   * cache[i-12]
                +          cache[i-13];
        cache[i] = v;
        ++cache_len;
    }

    return cache[n-1];                            /* cache index n‑1 corresponds to a(n) */
}
