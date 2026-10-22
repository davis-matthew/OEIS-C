/*
 * A373958  Number of compositions of 6*n-3 into parts 1 and 6.
 * https://oeis.org/A373958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of compositions of 6*n-3 into parts 1 and 6 (OEIS
 *            A373958) Recurrence (n >= 7): a(n) = 7*a(n-1) - 15*a(n-2) +
 *            20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6) Initial values:
 *            a(1)=1, a(2)=5, a(3)=21, a(4)=92, a(5)=414, a(6)=1869
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* array indexed from 1 */
static size_t cache_cap = 0;  /* allocated size (index 0 unused) */
static idx_t cache_max = 0;   /* highest index currently computed */

static const seq_t init_values[7] = {0ull, 1ull, 5ull, 21ull, 92ull, 414ull, 1869ull};

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, harmless */

    /* ensure sufficient capacity */
    if (cache == NULL || cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);   /* indices 0..n inclusive */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; return 0 as fallback */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing entries */
    if (cache_max < n) {
        /* fill from cache_max+1 up to n */
        idx_t i;
        for (i = cache_max + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = init_values[i];
            } else {
                /* recurrence: a(i) = 7*a(i-1) - 15*a(i-2) + 20*a(i-3)
                                 - 15*a(i-4) + 6*a(i-5) - a(i-6) */
                seq_t val = 7 * cache[i - 1];
                val -= 15 * cache[i - 2];
                val += 20 * cache[i - 3];
                val -= 15 * cache[i - 4];
                val += 6 * cache[i - 5];
                val -= cache[i - 6];
                cache[i] = val;
            }
        }
        cache_max = n;
    }

    return cache[n];
}
