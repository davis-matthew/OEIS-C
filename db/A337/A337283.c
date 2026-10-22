/*
 * A337283  a(n) = Sum_{i=0..n} i*T(i)^2, where T(i) = A000073(i) is the i-th
 *          tribonacci number.
 * https://oeis.org/A337283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} i*T(i)^2 where T(0)=0, T(1)=0, T(2)=1 and
 *            T(k)=T(k-1)+T(k-2)+T(k-3) for k≥3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *T = NULL;     /* tribonacci numbers */
static seq_t *a_cache = NULL; /* cumulative sums a(k) */
static idx_t cache_cap = 0;   /* capacity of the arrays */

/* Ensure that T and a_cache are computed up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmpT = (seq_t *)realloc(T, (size_t)new_cap * sizeof(seq_t));
    seq_t *tmpA = (seq_t *)realloc(a_cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmpT || !tmpA)
        abort(); /* allocation failed */

    T = tmpT;
    a_cache = tmpA;

    for (idx_t i = 0; i < new_cap; ++i) {
        if (i == 0 || i == 1)
            T[i] = (seq_t)0;
        else if (i == 2)
            T[i] = (seq_t)1;
        else
            T[i] = T[i-1] + T[i-2] + T[i-3];

        seq_t term = (seq_t)i * T[i] * T[i];
        if (i == 0)
            a_cache[i] = term;
        else
            a_cache[i] = a_cache[i-1] + term;
    }

    cache_cap = new_cap;
}

/* Main function required by the problem statement */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[(size_t)n];
}
