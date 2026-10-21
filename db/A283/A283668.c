/*
 * A283668  Numbers k such that 36k - 7, 36k - 6, 36k - 5, 36k - 3, 36k - 2,
 *          36k - 1, 36k + 1, 36k + 2, 36k + 3, 36k + 5, 36k + 6 and 36k + 7
 *          are all squarefree.
 * https://oeis.org/A283668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 527, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A283668: k such that all 12 numbers 36k±{7,6,5,3,2,1}
   are square‑free.  The function returns the n‑th such k (n≥1). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

/* Cached list of found terms (stored as uint64_t to avoid reallocting
   arbitrary‑precision objects).  The cache is lazily extended on demand. */
static uint64_t *cache = NULL;     /* array of k values (uint64_t) */
static size_t cache_len = 0;       /* number of entries cached */
static uint64_t last_k = 0;        /* largest k tested so far */

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; n==0 is never requested but guard it */
        return 0;

    if ((size_t)n > cache_len) {
        /* enlarge cache to hold at least n elements */
        size_t new_cap = (size_t)n;
        uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
        if (!tmp) {
            /* Out of memory – abort the program (acceptable for this sequence). */
            abort();
        }
        cache = tmp;

        /* test successive k values starting from last_k+1 */
        uint64_t k = last_k + 1;
        const int offs[12] = {-7, -6, -5, -3, -2, -1, 1, 2, 3, 5, 6, 7};

        while (cache_len < (size_t)n) {
            bool ok = true;
            seq_t base = (seq_t)k * 36;

            for (int i = 0; i < 12; ++i) {
                seq_t m = base + (seq_t)offs[i];
                if (!is_squarefree(m)) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                cache[cache_len] = k;
                ++cache_len;
            }
            ++k;
        }
        last_k = k - 1;   /* k has been incremented once after last test */
    }

    /* Return the cached k converted to seq_t */
    return (seq_t)cache[n - 1];
}
