/*
 * A302311  Number of 3 X n 0..1 arrays with every element equal to 0, 1, 3 or
 *          4 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) =
 *            a(n-1)+3*a(n-2)-a(n-3)+8*a(n-4)-7*a(n-5)-22*a(n-6)+9*a(n-7)
 *            -14*a(n-8)+10*a(n-9)+39*a(n-10)-19*a(n-11)+6*a(n-12)
 *            -18*a(n-14)+9*a(n-15) for n>16 Initial values: a(1)=4, a(2)=6,
 *            a(3)=13, a(4)=26, a(5)=48, a(6)=121, a(7)=275, a(8)=600,
 *            a(9)=1296, a(10)=2998, a(11)=6731, a...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[17] = {
    0,          /* unused; index 0 is dummy */
    4,          /* a(1) */
    6,          /* a(2) */
    13,         /* a(3) */
    26,         /* a(4) */
    48,         /* a(5) */
    121,        /* a(6) */
    275,        /* a(7) */
    600,        /* a(8) */
    1296,       /* a(9) */
    2998,       /* a(10) */
    6731,       /* a(11) */
    15022,      /* a(12) */
    33412,      /* a(13) */
    75480,      /* a(14) */
    169232,     /* a(15) */
    379265      /* a(16) */
};

/* Memoisation cache */
static seq_t *cache = NULL;
static size_t cache_size = 0;          /* number of allocated slots (including index 0) */

/* Ensure the cache can hold values up to 'n' (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n)
        return;
    size_t new_size = (size_t)n + 1;          /* indices 0..n */
    if (new_size < 17) new_size = 17;         /* at least up to initial values */
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();                  /* cannot recover */
    /* Fill new slots with zero; initial values are already present */
    if (new_size > cache_size)
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_size;
    /* Fill in the known initial values if they are not yet set */
    if (cache_size >= 17)
    {
        for (size_t i = 1; i <= 16; ++i)
            if (cache[i] == 0)
                cache[i] = init_vals[i];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for n=0, but guard */

    ensure_cache(n);

    if (cache[n] != 0)                        /* value already computed */
        return cache[n];

    /* Compute missing terms up to n */
    for (size_t i = 1; i <= (size_t)n; ++i)
    {
        if (cache[i] != 0)
            continue;                       /* already known */

        if (i <= 16)
        {
            cache[i] = init_vals[i];
            continue;
        }

        /* n > 16: use recurrence */
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* positive contributions */
        sum_pos += cache[i-1];
        sum_pos += cache[i-2] * (seq_t)3;
        sum_pos += cache[i-4] * (seq_t)8;
        sum_pos += cache[i-7] * (seq_t)9;
        sum_pos += cache[i-9] * (seq_t)10;
        sum_pos += cache[i-10] * (seq_t)39;
        sum_pos += cache[i-12] * (seq_t)6;
        sum_pos += cache[i-15] * (seq_t)9;

        /* negative contributions */
        sum_neg += cache[i-3];
        sum_neg += cache[i-5] * (seq_t)7;
        sum_neg += cache[i-6] * (seq_t)22;
        sum_neg += cache[i-8] * (seq_t)14;
        sum_neg += cache[i-11] * (seq_t)19;
        sum_neg += cache[i-14] * (seq_t)18;

        cache[i] = sum_pos - sum_neg;
    }

    return cache[n];
}
