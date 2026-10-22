/*
 * A357650  Expansion of e.g.f. cosh( (exp(4*x) - 1)/4 ).
 * https://oeis.org/A357650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 4^(n-2*k) * Stirling2(n,2*k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Ensure that *ptr points to an array of at least needed elements.
   New capacity grows by doubling. Newly allocated elements are zeroed. */
static void ensure_capacity(seq_t **ptr, size_t *current_cap, size_t needed)
{
    if (*current_cap >= needed)
        return;
    size_t new_cap = (*current_cap == 0) ? 1 : *current_cap;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* Zero any new elements */
    if (new_cap > *current_cap)
        memset(tmp + *current_cap, 0, (new_cap - *current_cap) * sizeof(seq_t));
    *ptr = tmp;
    *current_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static state persistent across calls */
    static seq_t *st          = NULL;  /* array holding current Stirling row */
    static size_t st_cap      = 0;     /* capacity of st */
    static seq_t *a_cache     = NULL;  /* cached a(n) values */
    static size_t a_cap       = 0;     /* capacity of a_cache */
    static idx_t  cached_n    = 0;     /* largest n already computed */

    /* Initialise on first use */
    if (st == NULL) {
        ensure_capacity(&st, &st_cap, 1);
        st[0] = (seq_t)1;              /* S(0,0) = 1 */
    }
    if (a_cache == NULL) {
        ensure_capacity(&a_cache, &a_cap, 1);
        a_cache[0] = (seq_t)1;         /* a(0) = 1 */
    }

    if (n == 0) return a_cache[0];

    /* Extend cache up to requested n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        size_t ui = (size_t)i;

        /* make sure storage is large enough */
        ensure_capacity(&st, &st_cap, ui + 1);
        ensure_capacity(&a_cache, &a_cap, ui + 1);

        /* compute Stirling numbers S(i,k) for k = i..1 in place */
        for (size_t k = ui; k >= 1; --k) {
            st[k] = (seq_t)k * st[k] + st[k - 1];
        }
        st[0] = (seq_t)0;  /* S(i,0) = 0 for i>0 */

        /* compute a(i) = sum_{even k<=i} 4^(i-k) * S(i,k) */
        seq_t sum   = (seq_t)0;
        size_t k_even = ui - (ui & 1);                 /* largest even ≤ i */
        seq_t pow4  = (ui & 1) ? (seq_t)4 : (seq_t)1;   /* 4^(i-k_even) */

        for (size_t k = k_even; ; k -= 2) {
            sum += pow4 * st[k];
            if (k == 0) break;
            pow4 *= (seq_t)16;  /* next exponent differs by 2: multiply by 4^2 */
        }
        a_cache[ui] = sum;
    }

    cached_n = n;
    return a_cache[n];
}
