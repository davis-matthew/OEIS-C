/*
 * A256804  Number of (n+2)X(2+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 0 and no antidiagonal sum 0 and no row sum 0 or 1 and no
 *          column sum 0 or 1.
 * https://oeis.org/A256804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = a(n-1) + 4*a(n-2)
 *            + 17*a(n-3) + 18*a(n-4) – 7*a(n-5) – 37*a(n-6) * – 47*a(n-7) +
 *            2*a(n-8) + 40*a(n-9) + 21*a(n-10) – 7*a(n-11) * – 3*a(n-12) –
 *            2*a(n-13) – a(n-14) * with a(1)=86, a(2)=312, a(3)=1167,
 *            a(4)=4219, a(5)=15291, a(6)=55798, * a(7)=203129, a...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[14] = {
    (seq_t)86,   (seq_t)312,   (seq_t)1167,  (seq_t)4219,   (seq_t)15291,
    (seq_t)55798, (seq_t)203129, (seq_t)738760, (seq_t)2689072, (seq_t)9788263,
    (seq_t)35621557, (seq_t)129642381, (seq_t)471844124, (seq_t)1717266695
};

static seq_t *cache = NULL;           /* immutable array 1‑based */
static idx_t  cache_used = 0;         /* cached size (number of slots) */

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n=0, but return 0 */

    /* initialise cache on first call */
    if (!cache) {
        /* allocate slots 0 … 14 (15 slots, slot 0 unused) */
        cache_used = 15;
        cache = (seq_t*)malloc(cache_used * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;                     /* unused, harmless */
        for (idx_t i = 1; i <= 14; ++i)
            cache[i] = init_vals[i - 1];
    }

    /* extend cache to cover requested index n */
    if (n >= cache_used) {
        size_t new_len = (size_t)n + 1;    /* number of slots needed */
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_used; i <= n; ++i) {
            seq_t a1  = cache[i-1];
            seq_t a2  = cache[i-2];
            seq_t a3  = cache[i-3];
            seq_t a4  = cache[i-4];
            seq_t a5  = cache[i-5];
            seq_t a6  = cache[i-6];
            seq_t a7  = cache[i-7];
            seq_t a8  = cache[i-8];
            seq_t a9  = cache[i-9];
            seq_t a10 = cache[i-10];
            seq_t a11 = cache[i-11];
            seq_t a12 = cache[i-12];
            seq_t a13 = cache[i-13];
            seq_t a14 = cache[i-14];

            seq_t pos = a1
                + (seq_t)4 * a2
                + (seq_t)17 * a3
                + (seq_t)18 * a4
                + (seq_t)2 * a8
                + (seq_t)40 * a9
                + (seq_t)21 * a10;

            seq_t neg = (seq_t)7  * a5
                + (seq_t)37 * a6
                + (seq_t)47 * a7
                + (seq_t)7  * a11
                + (seq_t)3  * a12
                + (seq_t)2  * a13
                + a14;

            cache[i] = pos - neg;
        }

        cache_used = n + 1;     /* now indices 0 … n are stored */
    }

    return cache[n];
}
