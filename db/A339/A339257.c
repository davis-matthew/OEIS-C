/*
 * A339257  Number of spanning trees in the n X 5 king graph.
 * https://oeis.org/A339257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the number of spanning trees in the n × 5 king graph:
 * a(1) = 1, a(2) = 27648, a(3) = 146356224, a(4) = 698512774464,
 * a(5) = 3271331573452800, a(6) = 15258885095892902976,
 * a(7) = 71111090441547013886784, a(8) = 331335100372867196224868352,
 * and for n > 8
 *
 * a(n) = 5889 a(n‑1) ‑ 5949540 a(n‑2) + 1033572501 a(n‑3)
 *       ‑ 51621632181 a(n‑4) + 930215250900 a(n‑5)
 *       ‑ 4819127400000 a(n‑6) + 4293081000000 a(n‑7)
 *       ‑ 656100000000 a(n‑8).
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

static const char * const init_str[12] = {
    "1",
    "27648",
    "146356224",
    "698512774464",
    "3271331573452800",
    "15258885095892902976",
    "71111090441547013886784",
    "331335100372867196224868352",
    "1543757070688065237574186369344",
    "7192607774929149127350811889484864",
    "33511424900308657559195109303117533184",
    "156134620449573478209362729027690283037248"
};

static const seq_t coeff[8] = {
    (seq_t)5889ULL,
    (seq_t)5949540ULL,
    (seq_t)1033572501ULL,
    (seq_t)51621632181ULL,
    (seq_t)930215250900ULL,
    (seq_t)4819127400000ULL,
    (seq_t)4293081000000ULL,
    (seq_t)656100000000ULL
};

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 12;
        cache_len  = 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        for (idx_t i = 0; i < 12; ++i)
            cache[i] = seq_from_str(init_str[i]);
    }

    /* compute new terms if needed */
    if (n > cache_len) {
        size_t needed = n;
        if (needed > cache_cap) {
            size_t new_cap = needed * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return cache[cache_len - 1];  /* realloc failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t cur = cache_len + 1; cur <= n; ++cur) {
            seq_t sum = 0;
            sum += cache[cur - 2] * coeff[0]; /* a(n‑1) */
            sum -= cache[cur - 3] * coeff[1]; /* a(n‑2) */
            sum += cache[cur - 4] * coeff[2]; /* a(n‑3) */
            sum -= cache[cur - 5] * coeff[3]; /* a(n‑4) */
            sum += cache[cur - 6] * coeff[4]; /* a(n‑5) */
            sum -= cache[cur - 7] * coeff[5]; /* a(n‑6) */
            sum += cache[cur - 8] * coeff[6]; /* a(n‑7) */
            sum -= cache[cur - 9] * coeff[7]; /* a(n‑8) */
            cache[cur - 1] = sum;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
