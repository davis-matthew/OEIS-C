/*
 * A298407  a(n) = 2*a(n-1) - a(n-3) + a(floor(n/2)) + a(floor(n/3)) + ... +
 *          a(floor(n/n)), where a(0) = 1, a(1) = 2, a(2) = 3.
 * https://oeis.org/A298407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑1) – a(n‑3) + Σ_{k=2}^{n} a(⌊n/k⌋) a(0) = 1, a(1) =
 *            2, a(2) = 3
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Ensure the cache array is large enough for indices up to `need`. */
static void grow_cache(idx_t need, seq_t **p, size_t *cap)
{
    if ((size_t)need <= *cap) return;
    size_t newcap = (*cap == 0) ? 4 : *cap;
    while (newcap < (size_t)need) newcap <<= 1;   /* double */
    seq_t *tmp = (seq_t *)realloc(*p, newcap * sizeof(seq_t));
    if (!tmp) exit(1);   /* allocation failure */
    *p = tmp;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* highest index that has been computed */
    static size_t capacity = 0;

    /* initialise base values on first call */
    if (cache == NULL) {
        grow_cache(3, &cache, &capacity);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        max_computed = 2;
    }

    if (n <= max_computed) return cache[(size_t)n];

    /* grow cache to hold indices up to n */
    grow_cache((idx_t)(n + 1), &cache, &capacity);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t k = 2;
        while (k <= i) {
            idx_t q = i / k;          /* floor(i/k)           */
            idx_t j = i / q;          /* last k with this q   */
            seq_t cnt = (seq_t)(j - k + 1);
            sum += cache[(size_t)q] * cnt;
            k = j + 1;
        }
        /* recurrence: a(i) = 2*a(i-1) – a(i-3) + sum */
        seq_t val = (seq_t)2 * cache[(size_t)(i - 1)] - cache[(size_t)(i - 3)] + sum;
        cache[(size_t)i] = val;
    }

    max_computed = n;
    return cache[(size_t)n];
}
