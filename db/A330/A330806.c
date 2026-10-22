/*
 * A330806  a(1) = 1; a(2) = 1; for n >= 3, a(n) = a(n-1) / gcd(a(n-1), n-1) +
 *          a(n-2) / gcd(a(n-2), n-2).
 * https://oeis.org/A330806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, for n >= 3: a(n) = a(n-1)/gcd(a(n-1), n-1) +
 *            a(n-2)/gcd(a(n-2), n-2)
 * Range:     u8 n <= 23, u16 n <= 43, u32 n <= 86, u64 n <= 180,
 *            u128 n <= 388, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* static cache: cache[i] holds a(i) for 1 <= i <= last */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static idx_t cap = 0;         /* capacity of the allocated array (index 1 .. cap-1 usable) */
    static idx_t last = 0;        /* highest index computed so far */

    if (n == 0) return 0;         /* undefined, but return 0 */

    /* first allocation and initialization */
    if (cache == NULL) {
        cap = 4;                  /* start with modest capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0;     /* allocation failure */
        for (idx_t i = 0; i < cap; ++i) cache[i] = 0;  /* zero init */
        cache[1] = 1;
        cache[2] = 1;
        last = 2;
    }

    /* ensure capacity for index n */
    if ((idx_t)(cap - 1) < n) {
        size_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        for (idx_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* compute values up to n if not already computed */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 1 || i == 2) {
                cache[i] = 1;
                continue;
            }
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t g1 = gcd(prev1, (seq_t)(i - 1));
            seq_t g2 = gcd(prev2, (seq_t)(i - 2));
            cache[i] = prev1 / g1 + prev2 / g2;
        }
        last = n;
    }

    return cache[n];
}
