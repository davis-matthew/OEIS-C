/*
 * A300316  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          5, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 17 a(n) = 3 a(n‑1) + 5 a(n‑2) + 14 a(n‑3) – 45 a(n‑4) –
 *            51 a(n‑5) – 3 a(n‑6) +122 a(n‑7) +106 a(n‑8) – 48 a(n‑9) – 52
 *            a(n‑10) – 18 a(n‑11) + 24 a(n‑12) – 75 a(n‑13) – 4 a(n‑14) + 30
 *            a(n‑15) Initial values: a(1)=3, a(2)=4, a(3)=16, a(4)=61,
 *            a(5)=186, a(6)=977, a(7)=3875, a(8)=15976, a(9)=6...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t initial[17] = {
    (seq_t)3,          // a(1)
    (seq_t)4,          // a(2)
    (seq_t)16,         // a(3)
    (seq_t)61,         // a(4)
    (seq_t)186,        // a(5)
    (seq_t)977,        // a(6)
    (seq_t)3875,       // a(7)
    (seq_t)15976,      // a(8)
    (seq_t)69695,      // a(9)
    (seq_t)291860,     // a(10)
    (seq_t)1231606,    // a(11)
    (seq_t)5238686,    // a(12)
    (seq_t)22132400,   // a(13)
    (seq_t)93661059,   // a(14)
    (seq_t)396772953,  // a(15)
    (seq_t)1679005708, // a(16)
    (seq_t)7107319506  // a(17)
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, guard */

    static seq_t *cache = NULL;   /* cache of computed terms */
    static size_t cached_n = 0;   /* number of terms currently cached */

    /* enlarge the cache if necessary */
    if ((size_t)n > cached_n) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)n * sizeof(seq_t));
        if (!new_cache)              /* allocation failure – should not
                                         happen in typical tests */
            return (seq_t)0;
        cache = new_cache;

        /* compute missing terms */
        for (size_t i = cached_n + 1; i <= n; ++i) {
            size_t idx = i - 1;   /* zero based index into cache */
            if (i <= 17) {                      /* base cases */
                cache[idx] = initial[i - 1];
            } else { /* recurrence for i > 17 */
                seq_t val =
                      (seq_t)3   * cache[idx - 1]   /* a(n‑1) */
                    + (seq_t)5   * cache[idx - 2]   /* a(n‑2) */
                    + (seq_t)14  * cache[idx - 3]   /* a(n‑3) */
                    - (seq_t)45  * cache[idx - 4]   /* a(n‑4) */
                    - (seq_t)51  * cache[idx - 5]   /* a(n‑5) */
                    - (seq_t)3   * cache[idx - 6]   /* a(n‑6) */
                    + (seq_t)122 * cache[idx - 7]   /* a(n‑7) */
                    + (seq_t)106 * cache[idx - 8]   /* a(n‑8) */
                    - (seq_t)48  * cache[idx - 9]   /* a(n‑9) */
                    - (seq_t)52  * cache[idx -10]   /* a(n‑10) */
                    - (seq_t)18  * cache[idx -11]   /* a(n‑11) */
                    + (seq_t)24  * cache[idx -12]   /* a(n‑12) */
                    - (seq_t)75  * cache[idx -13]   /* a(n‑13) */
                    - (seq_t)4   * cache[idx -14]   /* a(n‑14) */
                    + (seq_t)30  * cache[idx -15];  /* a(n‑15) */
                cache[idx] = val;
            }
        }
        cached_n = n;
    }

    return cache[n - 1];
}
