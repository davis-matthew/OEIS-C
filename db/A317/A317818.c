/*
 * A317818  Number of nX3 0..1 arrays with every element unequal to 1, 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = a(n-1) +7*a(n-2) +2*a(n-3)
 *            -10*a(n-4) -14*a(n-5) -16*a(n-6) -25*a(n-7) +36*a(n-8)
 *            +42*a(n-9) -26*a(n-10) +62*a(n-11) +10*a(n-12) -28*a(n-13)
 *            +20*a(n-14) -16*a(n-15) +4*a(n-16) for n > 18. Initial values (n
 *            = 1 … 18): a(1)=1, a(2)=16, a(3)=35, a(4)=135, a(5)=356,
 *            a(6)=1...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic cache of a(n)   */
static size_t cache_size = 0;        /* allocated size (indexable up to cache_size-1) */
static idx_t max_computed = 0;       /* largest n for which cache[n] is defined */

/* initial values (1‑based indexing) */
static const seq_t base_vals[19] = {
    0ULL,            /* dummy for index 0 */
    1ULL,            /* a(1) */
    16ULL,           /* a(2) */
    35ULL,           /* a(3) */
    135ULL,          /* a(4) */
    356ULL,          /* a(5) */
    1113ULL,         /* a(6) */
    3150ULL,         /* a(7) */
    9484ULL,         /* a(8) */
    27522ULL,        /* a(9) */
    81798ULL,        /* a(10) */
    239572ULL,       /* a(11) */
    707803ULL,       /* a(12) */
    2080838ULL,      /* a(13) */
    6134691ULL,      /* a(14) */
    18056895ULL,     /* a(15) */
    53200116ULL,     /* a(16) */
    156648460ULL,    /* a(17) */
    461423021ULL     /* a(18) */
};

/* ensure cache large enough and pre‑initialise base terms if first use */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_size)
        return;                           /* already large enough */

    size_t new_size = (size_t)(n + 1);
    if (new_size < 19) new_size = 19;       /* at least store first 18 terms */

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();                           /* allocation failure */

    /* copy existing values */
    for (size_t i = 1; i < cache_size; ++i)
        new_cache[i] = cache[i];

    cache = new_cache;
    cache_size = new_size;

    /* initialise base values if they are not already present */
    for (size_t i = 1; i < 19 && i < cache_size; ++i)
    {
        if (cache[i] == 0)
            cache[i] = base_vals[i];
    }
    if (max_computed < 18 && cache_size > 18)
        max_computed = 18;
}

/* compute terms up to n using the recurrence */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed)
        return;

    for (idx_t k = max_computed + 1; k <= n; ++k)
    {
        seq_t val;
        if (k <= 18)          /* base values already in cache */
        {
            val = cache[k];
        }
        else
        {
            val = 0;
            val += cache[k - 1];
            val += (seq_t)7 * cache[k - 2];
            val += (seq_t)2 * cache[k - 3];
            val -= (seq_t)10 * cache[k - 4];
            val -= (seq_t)14 * cache[k - 5];
            val -= (seq_t)16 * cache[k - 6];
            val -= (seq_t)25 * cache[k - 7];
            val += (seq_t)36 * cache[k - 8];
            val += (seq_t)42 * cache[k - 9];
            val -= (seq_t)26 * cache[k -10];
            val += (seq_t)62 * cache[k -11];
            val += (seq_t)10 * cache[k -12];
            val -= (seq_t)28 * cache[k -13];
            val += (seq_t)20 * cache[k -14];
            val -= (seq_t)16 * cache[k -15];
            val += (seq_t)4 * cache[k -16];
        }
        cache[k] = val;
    }
    max_computed = n;
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n <= 0 */
    ensure_cache(n);
    compute_up_to(n);
    return cache[n];
}
