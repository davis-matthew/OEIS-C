/*
 * A390035  a(n) = Sum_{k=0..floor(n/2)} binomial(k+2,3*n-6*k+2).
 * https://oeis.org/A390035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 35, u32 n <= 70, u64 n <= 139,
 *            u128 n <= 277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A390035
   Formula: a(n) = Σ_{k=0..⌊n/2⌋} binomial(k+2, 3*n-6*k+2)
   Recurrence (for n≥7): a(n) = 3*a(n-2) - 3*a(n-4) + a(n-6) + a(n-7)
   Initial values: a(0)=1, a(1)=0, a(2)=3, a(3)=0, a(4)=6, a(5)=0, a(6)=10
 */

#include <stdlib.h>

/*
   Computes a(n) for the sequence A390035.
   The function caches previously computed values so that any
   call order is supported and each value is computed only once.
*/
seq_t a(idx_t n)
{
    /* static cache: sequential array of computed terms              */
    static seq_t *cache   = NULL;   /* allocated array of seq_t          */
    static size_t cache_cap = 0;    /* current allocated capacity (count)*/
    static idx_t   cache_len = 0;   /* number of terms actually stored   */

    /* enlarge the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failure – return 0 as a safe default */
            return (seq_t)0;
        }
        /* copy existing values */
        if (cache_len > 0)
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
        /* free old memory if any */
        if (cache)
            free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 7) {                     /* use the known initial values */
            switch (i) {
                case 0: cache[i] = (seq_t)1; break;
                case 1: cache[i] = (seq_t)0; break;
                case 2: cache[i] = (seq_t)3; break;
                case 3: cache[i] = (seq_t)0; break;
                case 4: cache[i] = (seq_t)6; break;
                case 5: cache[i] = (seq_t)0; break;
                case 6: cache[i] = (seq_t)10; break;
            }
        } else { /* recurrence for i ≥ 7 */
            cache[i] = ((seq_t)3) * cache[i - 2]
                      - ((seq_t)3) * cache[i - 4]
                      + cache[i - 6]
                      + cache[i - 7];
        }
    }

    /* keep track of how many terms we have stored */
    cache_len = n + 1;

    return cache[n];
}
