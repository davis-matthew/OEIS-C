/*
 * A393908  a(n) = Sum_{i=0..n} Sum_{j=0..i} binomial(n+2*i,3*i) *
 *          binomial(i+2*j,3*j).
 * https://oeis.org/A393908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 63*a(n-2) + 154*a(n-3) - 215*a(n-4) +
 *            189*a(n-5) - 108*a(n-6) + 40*a(n-7) - 9*a(n-8) + a(n-9) with
 *            a(0)=1, a(1)=3, a(2)=15, a(3)=82, a(4)=459, a(5)=2589,
 *            a(6)=14632, a(7)=82708, a(8)=467399
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compile with -std=c11 or -std=c++17. */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;         /* array of computed terms */
static size_t cache_cap = 0;        /* allocated capacity   */
static size_t cache_len = 0;        /* valid indices 0..cache_len-1 */

/* Initial 9 terms of the sequence. */
static const seq_t base_vals[9] = {
    (seq_t)1, (seq_t)3, (seq_t)15, (seq_t)82,
    (seq_t)459, (seq_t)2589, (seq_t)14632,
    (seq_t)82708, (seq_t)467399
};

/* Ensure cache has space for at least 'needed' entries (0‑based). */
static void grow_cache(idx_t needed)
{
    if ((size_t)needed < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 9;
    while (new_cap <= (size_t)needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* memory allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise cache with the first nine terms. */
static void init_cache(void)
{
    if (cache != NULL) return;
    cache = (seq_t*)malloc(9 * sizeof(seq_t));
    if (!cache) abort();
    for (size_t i = 0; i < 9; ++i) cache[i] = base_vals[i];
    cache_cap = 9;
    cache_len = 9;
}

seq_t a(idx_t n)
{
    init_cache();                  /* ensure base values stored */

    if (n < cache_len)             /* already computed */
        return cache[n];

    /* Ensure enough space to store terms up to n. */
    grow_cache(n + 1);

    /* Coefficients of the recurrence.  These are small constants
       and fit comfortably in seq_t. */
    const seq_t C13  = (seq_t)13,  C154 = (seq_t)154, C189 = (seq_t)189,
                 C40   = (seq_t)40,  C1   = (seq_t)1;
    const seq_t D63  = (seq_t)63,  D215 = (seq_t)215, D108 = (seq_t)108,
                 D9   = (seq_t)9;

    /* Compute terms from cache_len up to n inclusive. */
    for (idx_t k = cache_len; k <= n; ++k)
    {
        seq_t sum_pos = cache[k-1] * C13
                       + cache[k-3] * C154
                       + cache[k-5] * C189
                       + cache[k-7] * C40
                       + cache[k-9] * C1;

        seq_t sum_neg = cache[k-2] * D63
                       + cache[k-4] * D215
                       + cache[k-6] * D108
                       + cache[k-8] * D9;

        cache[k] = sum_pos - sum_neg;
    }

    cache_len = n + 1;
    return cache[n];
}
