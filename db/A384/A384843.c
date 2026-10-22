/*
 * A384843  Wiener index of the n-Dorogovtsev-Goltsev-Mendes graph.
 * https://oeis.org/A384843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384843: Wiener index of the n-Dorogovtsev-Goltsev-Mendes graph
 *
 * Formula:
 *   a(0) = 1
 *   a(n) = 22*a(n-1) - 156*a(n-2) + 378*a(n-3) - 243*a(n-4)   for n > 4
 *   (with initial values a(1)=3, a(2)=21, a(3)=204, a(4)=2130)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: sequence values already computed */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;           /* number of elements currently stored */

    /* enlarge cache if needed to hold index n */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
#if defined(__cplusplus)
        seq_t *tmp = static_cast<seq_t*>(realloc(cache, new_len * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
#endif
        cache = tmp;

        /* Initialise base values */
        static const seq_t base[5] = {
            (seq_t)1,     /* a(0) */
            (seq_t)3,     /* a(1) */
            (seq_t)21,    /* a(2) */
            (seq_t)204,   /* a(3) */
            (seq_t)2130   /* a(4) */
        };

        /* Compute missing values */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 4) {
                cache[i] = base[i];
            } else {
                seq_t t1 = (seq_t)22 * cache[i-1];
                seq_t t2 = (seq_t)156 * cache[i-2];
                seq_t t3 = (seq_t)378 * cache[i-3];
                seq_t t4 = (seq_t)243 * cache[i-4];
                cache[i] = t1 - t2 + t3 - t4;
            }
        }

        cache_len = new_len;
    }

    return cache[n];
}
