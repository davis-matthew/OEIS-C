/*
 * A387456  Number of Hamiltonian paths in the n-flower graph.
 * https://oeis.org/A387456
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 4*a(n-1)-16*a(n-3)+14*a(n-4)+8*a(n-5)-16*a(n-6)
 *            +32*a(n-7)-25*a(n-8)-28*a(n-9)+32*a(n-10)-16*a(n-11)
 *            +12*a(n-12)+16*a(n-13)-16*a(n-14) (Recurrence for n ≥ 16, with
 *            a(2)…a(15) as base values below)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 54, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Base values a(2)…a(15) */
static const seq_t base[14] = {
    (seq_t)48,      /* a(2)  */
    (seq_t)192,     /* a(3)  */
    (seq_t)696,     /* a(4)  */
    (seq_t)1780,    /* a(5)  */
    (seq_t)4824,    /* a(6)  */
    (seq_t)11368,   /* a(7)  */
    (seq_t)27552,   /* a(8)  */
    (seq_t)62064,   /* a(9)  */
    (seq_t)141840,  /* a(10) */
    (seq_t)312224,  /* a(11) */
    (seq_t)690768,  /* a(12) */
    (seq_t)1496768, /* a(13) */
    (seq_t)3246096, /* a(14) */
    (seq_t)6956160  /* a(15) */
};

/* Coefficients for the recurrence */
static const int  pos_coefs[7] = { 4, 14, 8, 32, 32, 12, 16 };
static const int  neg_coefs[6] = {16, 16, 25, 28, 16, 16};
static const int  pos_shifts[7] = { 1, 4, 5, 7, 10, 12, 13 };
static const int  neg_shifts[6] = { 3, 6, 8, 9, 11, 14 };

/* Cache for computed terms a(2), a(3), … */
static seq_t *cache   = NULL;   /* Pointer to array of terms */
static size_t cache_size = 0;   /* Number of terms stored */
static size_t cache_cap  = 0;   /* Allocated capacity */

/* Grow the cache to at least the required capacity */
static void
ensure_cache_capacity(size_t required_cap)
{
    if (required_cap <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < required_cap) new_cap <<= 1;   /* double */

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache) exit(1); /* allocation failure */

    /* copy old data */
    for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];

    /* free old buffer */
#ifdef __cplusplus
    delete [] cache;
#else
    free(cache);
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure the cache contains all terms up to a(n) (inclusive) */
static void
ensure_cache_terms(idx_t n)
{
    if (n < 2) return;                 /* nothing to compute */

    /* Number of terms needed: a(2)…a(n) inclusive */
    size_t needed = (size_t)(n - 1);
    if (cache_size >= needed) return;  /* already have them */

    /* Ensure we have enough space */
    ensure_cache_capacity(needed);

    /* If cache empty, load the base values */
    if (cache_size == 0) {
        size_t fill = 14;              /* number of base values */
        for (size_t i = 0; i < fill; ++i) cache[i] = base[i];
        cache_size = fill;
    }

    /* Compute further terms */
    for (size_t idx = cache_size; idx < needed; ++idx) {
        seq_t pos_sum = 0, neg_sum = 0;

        /* positive contributions */
        for (int i = 0; i < 7; ++i)
            pos_sum += (seq_t)pos_coefs[i] * cache[idx - pos_shifts[i]];

        /* negative contributions */
        for (int i = 0; i < 6; ++i)
            neg_sum += (seq_t)neg_coefs[i] * cache[idx - neg_shifts[i]];

        cache[idx] = pos_sum - neg_sum;
    }

    cache_size = needed;
}

/* Public function computing the sequence */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;                 /* undefined below offset */

    if (n <= 15) return base[n - 2];            /* direct lookup */

    /* Ensure we have all terms up to n */
    ensure_cache_terms(n);

    return cache[n - 2];                        /* zero‑based index */
}
