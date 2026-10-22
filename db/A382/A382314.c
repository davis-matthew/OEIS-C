/*
 * A382314  G.f. satisfies A(x) = 1/(1-x) + 2*x*A(x^2) + 3*x^2*A(x^3).
 * https://oeis.org/A382314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n % 2 == 1 ? 2*a((n-1)/2) : 0) + (n % 3 == 2 ?
 *            3*a((n-2)/3) : 0) with a(0)=1
 * Range:     u8 n <= 34, u16 n <= 862, u32 n <= 1030, u64 n <= 1030,
 *            u128 n <= 1030, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds all values up to the largest n seen so far */
    static seq_t *cache = NULL;     /* pointer to array of computed values */
    static size_t cache_cap = 0;    /* current capacity of cache */
    static size_t computed = 0;     /* largest index computed (inclusive) */
    static int initialized = 0;     /* initialise cache only once */

    if (!initialized) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;                  /* a(0)=1 */
        computed = 0;
        initialized = 1;
    }

    /* enlarge cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)1;                /* constant 1 */

        if (i & 1) {                         /* n odd → use (n-1)/2 */
            size_t j = (i - 1) / 2;
            val += (seq_t)2 * cache[j];
        }

        if (i % 3 == 2) {                    /* n ≡ 2 mod 3 → use (n-2)/3 */
            size_t j = (i - 2) / 3;
            val += (seq_t)3 * cache[j];
        }

        cache[i] = val;
    }
    if (computed < (size_t)n) computed = (size_t)n;

    return cache[(size_t)n];
}
