/*
 * A337490  a(0)=1; for n > 0, a(n) = the greatest common divisor (GCD) of n
 *          and the sum of all previous terms if the GCD is not already in the
 *          sequence; otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A337490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, let S=∑_{k=0}^{n-1}a(k). If gcd(n,S) has not
 *            yet appeared in the sequence, a(n)=gcd(n,S); otherwise
 *            a(n)=a(n-1)+n
 * Range:     u8 n <= 17, u16 n <= 170, u32 n <= 4639, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t cache_cap = 0;
    static idx_t max_cached = 0;   /* highest index computed */
    static seq_t sum = 0;          /* Σ_{k=0}^{max_cached} a(k) */

    /* initialise cache on first call */
    if (terms == NULL)
    {
        cache_cap = 1;
        terms = seq_alloc(cache_cap);
        terms[0] = 1;   /* a(0) = 1 */
        sum = 1;
        max_cached = 0;
    }

    /* if requested term already computed, return it */
    if (n <= max_cached)
        return terms[n];

    /* enlarge cache if necessary */
    size_t required = (size_t)(n + 1);
    if (required > cache_cap)
    {
        terms = seq_realloc(terms, cache_cap, required);
        cache_cap = required;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        seq_t g = gcd((seq_t)i, sum);

        bool exists = false;
        for (idx_t j = 0; j < i; ++j)
        {
            if (terms[j] == g)
            {
                exists = true;
                break;
            }
        }

        seq_t next = exists ? (terms[i - 1] + i) : g;
        terms[i] = next;
        sum += next;
    }

    max_cached = n;
    return terms[n];
}
