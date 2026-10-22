/*
 * A370391  Expansion of (1 - 2*x)/(1 - 9*x + 28*x^2 - 35*x^3 + 15*x^4 - x^5).
 * https://oeis.org/A370391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5)
 *            with a(0)=1, a(1)=7, a(2)=35, a(3)=154, a(4)=636
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  computed = 0;      /* number of terms currently stored (index+1) */
    static size_t cap = 0;          /* allocated capacity */

    /* initial few terms */
    static const seq_t init[5] = { (seq_t)1,
                                   (seq_t)7,
                                   (seq_t)35,
                                   (seq_t)154,
                                   (seq_t)636 };

    /* enlarge cache if needed */
    if (n >= computed) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t new_cap = cap == 0 ? 8 : cap * 2;
            while (new_cap < need) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { free(cache); abort(); }
            cache = tmp;
            cap = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = computed; i <= n; ++i) {
            if (i < 5) {
                cache[i] = init[i];
            } else {
                seq_t t = 0;
                t += (seq_t)9  * cache[i-1];
                t -= (seq_t)28 * cache[i-2];
                t += (seq_t)35 * cache[i-3];
                t -= (seq_t)15 * cache[i-4];
                t +=          cache[i-5];
                cache[i] = t;
            }
        }
        computed = (idx_t)n + 1;
    }

    return cache[n];
}
