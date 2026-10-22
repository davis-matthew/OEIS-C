/*
 * A387850  a(n) = Sum_{k=0..n} binomial(5*n,6*k).
 * https://oeis.org/A387850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*n, 6*k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (valid for n > 5):
   a(n) = 6*a(n-1) + 615*a(n-2) + 7160*a(n-3) - 7155*a(n-4) + 7776*a(n-5)
   with initial values:
   a(0)=1, a(1)=1, a(2)=211, a(3)=5461, a(4)=164921, a(5)=5858126
*/

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached_up_to = 0;   /* number of cached elements */

/* ensure that cache contains entries for indices 0..n */
static void ensure_cache(idx_t n)
{
    if (n < cached_up_to)
        return;

    /* determine required capacity (power of two >= n+1) */
    size_t new_cap = (cached_up_to == 0) ? 1 : cached_up_to;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    /* allocate or reallocate memory */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) /* allocation failed – abort */
        abort();
    cache = new_cache;

    /* fill new entries */
    for (idx_t i = cached_up_to; i <= n; ++i) {
        if (i <= 5) { /* base cases */
            switch (i) {
                case 0: cache[0] = (seq_t)1; break;
                case 1: cache[1] = (seq_t)1; break;
                case 2: cache[2] = (seq_t)211; break;
                case 3: cache[3] = (seq_t)5461; break;
                case 4: cache[4] = (seq_t)164921; break;
                case 5: cache[5] = (seq_t)5858126; break;
            }
        } else { /* recurrence */
            seq_t t = (seq_t)6  * cache[i-1]
                    + (seq_t)615 * cache[i-2]
                    + (seq_t)7160 * cache[i-3]
                    + (seq_t)7776 * cache[i-5];
            t -= (seq_t)7155 * cache[i-4];
            cache[i] = t;
        }
    }

    cached_up_to = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
