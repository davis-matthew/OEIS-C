/*
 * A297696  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A297696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +10*a(n-2) -24*a(n-3) -24*a(n-4) +43*a(n-5)
 *            +22*a(n-6) -17*a(n-7) -4*a(n-8) +2*a(n-9) Initial values:
 *            a(1)=8, a(2)=19, a(3)=95, a(4)=293, a(5)=1244, a(6)=4277,
 *            a(7)=16931, a(8)=61040, a(9)=234262
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[10] = {
    0,      /* unused 0‑th element */
    8,      /* a(1) */
    19,     /* a(2) */
    95,     /* a(3) */
    293,    /* a(4) */
    1244,   /* a(5) */
    4277,   /* a(6) */
    16931,  /* a(7) */
    61040,  /* a(8) */
    234262  /* a(9) */
};

/* Internal cache for previously computed terms.
   cache[i] holds a(i) for 1 ≤ i ≤ cache_size. */
static seq_t *cache = NULL;         /* null until first call */
static idx_t cache_size = 0;        /* largest index currently cached */

/* Initialise the cache with the first nine terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache = (seq_t *)malloc((9 + 1) * sizeof(seq_t));
    if (cache == NULL)
        return;                    /* allocation failure – caller sees 0 */

    for (idx_t i = 1; i <= 9; ++i)
        cache[i] = base_vals[i];
    cache_size = 9;
}

/* Compute a(n) using the recurrence, extending the cache as needed. */
seq_t a(idx_t n)
{
    if (n == 0)       /* the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Ensure base terms are in the cache */
    init_cache();

    /* Return directly if n is within the cached range */
    if (n <= cache_size)
        return cache[n];

    /* Grow cache to have room for the new index */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL)
        return (seq_t)0;    /* allocation failure */
    cache = new_cache;

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = cache[i - 1] * 3;
        pos += cache[i - 2] * 10;
        pos += cache[i - 5] * 43;
        pos += cache[i - 6] * 22;
        pos += cache[i - 9] * 2;

        seq_t neg = cache[i - 3] * 24;
        neg += cache[i - 4] * 24;
        neg += cache[i - 7] * 17;
        neg += cache[i - 8] * 4;

        cache[i] = pos - neg;   /* guaranteed non‑negative for n ≥ 10 */
    }

    cache_size = n;
    return cache[n];
}
