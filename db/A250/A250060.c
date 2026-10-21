/*
 * A250060  Number of length 1+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining five terms.
 * https://oeis.org/A250060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) for n > 8, with a(1)=21,
 *            a(2)=735, a(3)=7224, a(4)=40320, a(5)=160545, a(6)=509691,
 *            a(7)=1375080, a(8)=3281544
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values, index = n */
static idx_t  cache_size = 0;        /* highest n stored, 0 if none */

/* Ensure that cache contains values up to n (alloc/grow as needed). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)              /* already computed */
        return;

    /* Start with space for at least the first eight terms. */
    idx_t need = (n < 8 ? 8 : n);

    if (cache == NULL) {
        /* first allocation: pre-fill the initial terms. */
        cache = (seq_t *)malloc((need + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;                     /* unused sentinel */
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)735;
        cache[3] = (seq_t)7224;
        cache[4] = (seq_t)40320;
        cache[5] = (seq_t)160545;
        cache[6] = (seq_t)509691;
        cache[7] = (seq_t)1375080;
        cache[8] = (seq_t)3281544;
        cache_size = 8;
    } else {
        /* grow the array if required. */
        cache = (seq_t *)realloc(cache, (need + 1) * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Compute any new terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 8)                     /* base terms already set */
            continue;

        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        seq_t a7 = cache[i - 7];
        seq_t a8 = cache[i - 8];

        /* Compute positive and negative contributions separately to avoid
           unsigned underflow during subtraction. */
        seq_t pos = 8 * a1 + 56 * a3 + 56 * a5 + 8 * a7;
        seq_t neg = 28 * a2 + 70 * a4 + 28 * a6 + a8;
        cache[i] = pos - neg;
    }

    cache_size = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1; return 0 */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
