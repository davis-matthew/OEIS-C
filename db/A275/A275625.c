/*
 * A275625  Numerator of Sum_{k=1..n} 0.k.
 * https://oeis.org/A275625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, frac
 * Formula:   a(n) = numerator of Sum_{k=1..n} (k / 10^{digits(k)}) in lowest
 *            terms
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 1401, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   digits_of, gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digits_of.h"
#include "../utils/pow10.h"
#include "../utils/gcd.h"

/* Return the numerator of the sum of k / 10^{digits(k)} for k=1..n.  */
seq_t a(idx_t n)
{
    /* Static cache to support arbitrary access patterns.  */
    static seq_t *cache = NULL;     /* numerators */
    static size_t cap = 0, len = 0; /* cache size and filled length */
    static idx_t current_idx = 0;   /* last computed index (0 == none) */
    static seq_t current_num = 0;   /* current numerator */
    static seq_t current_den = 1;   /* current denominator */

    /* Return cached value if possible.  */
    if (n <= current_idx && n > 0)
        return cache[n - 1];

    /* Grow cache as needed.  */
    if (n > len) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) {
            if (new_cap == 0) new_cap = 16;
            else new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms from current_idx+1 up to n.  */
    for (idx_t i = current_idx + 1; i <= n; ++i) {
        /* Number of decimal digits in i.  */
        int d = digits_of(i);
        seq_t pow10_d = pow10((idx_t)d);

        /* New numerator and denominator before reduction.  */
        seq_t new_num = current_num * pow10_d + (seq_t)i * current_den;
        seq_t new_den = current_den * pow10_d;

        /* Reduce fraction.  */
        seq_t g = gcd(new_num, new_den);
        current_num = new_num / g;
        current_den = new_den / g;

        /* Store numerator for cache.  */
        cache[i - 1] = current_num;
    }

    current_idx = n;
    return cache[n - 1];
}
