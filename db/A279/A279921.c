/*
 * A279921  Number of nX2 0..2 arrays with no element equal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with the exception of exactly two elements, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A279921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A279921
   a(n) = 24a(n-1) -192a(n-2) +500a(n-3) +204a(n-4) -996a(n-5)
          +1248a(n-6) -1056a(n-7) -4176a(n-8) +2528a(n-9) +912a(n-10)
          -1536a(n-11) +8896a(n-12) +5376a(n-13) -3648a(n-14)
          -192a(n-15) -3840a(n-16) -9216a(n-17) -4096a(n-18)
   for n > 19, with the first 19 terms defined below. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* Cache of computed terms (0‑based offset) */
static size_t cache_size = 0;    /* Number of terms currently stored        */
static size_t cache_cap   = 0;    /* Current allocated capacity in cache     */

/* Grow the cache to at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        abort(); /* Out of memory – no recovery path needed for OEIS tests. */
    if (cache != NULL)
    {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* 18‑term coefficients of the empirical recurrence. */
static const int coeffs[18] = {
    24, -192, 500, 204, -996, 1248, -1056, -4176,
    2528, 912, -1536, 8896, 5376, -3648, -192, -3840, -9216, -4096
};

/* Base values for indices 1..19 (stored 0‑based as base[0] = a(1)). */
static const seq_t base[19] = {
    (seq_t)1ULL,          /* a(1) */
    (seq_t)0ULL,          /* a(2) */
    (seq_t)15ULL,         /* a(3) */
    (seq_t)142ULL,        /* a(4) */
    (seq_t)1350ULL,       /* a(5) */
    (seq_t)12858ULL,      /* a(6) */
    (seq_t)119396ULL,     /* a(7) */
    (seq_t)1095012ULL,    /* a(8) */
    (seq_t)9924024ULL,    /* a(9) */
    (seq_t)89076240ULL,   /* a(10) */
    (seq_t)792954264ULL,  /* a(11) */
    (seq_t)7008783648ULL, /* a(12) */
    (seq_t)61565762448ULL,/* a(13) */
    (seq_t)537846507200ULL,/* a(14) */
    (seq_t)4675888234720ULL,/* a(15) */
    (seq_t)40474106338144ULL,/* a(16) */
    (seq_t)348966901404864ULL,/* a(17) */
    (seq_t)2998086166443584ULL,/* a(18) */
    (seq_t)25674017271458048ULL/* a(19) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* Undefined for n<1; return 0 to satisfy type system */

    /* Return cached value if already computed. */
    if (cache_size >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Ensure cache capacity. */
    grow_cache((size_t)n);

    /* Compute terms one by one up to the requested index. */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i)
    {
        seq_t val;
        if (i <= 19)                  /* Base values for n = 1..19. */
            val = base[i - 1];
        else
        {
            seq_t pos_sum = 0, neg_sum = 0;
            for (int j = 0; j < 18; ++j)
            {
                seq_t term = cache[i - 2 - j];
                int c = coeffs[j];
                if (c > 0)
                    pos_sum += (seq_t)(unsigned int)c * term;
                else
                    neg_sum += (seq_t)(unsigned int)(-c) * term;
            }
            val = pos_sum - neg_sum;  /* Safe: final value is non‑negative. */
        }
        cache[i - 1] = val;
    }
    cache_size = (size_t)n;
    return cache[(size_t)n - 1];
}
