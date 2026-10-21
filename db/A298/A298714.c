/*
 * A298714  Number of nX3 0..1 arrays with every element equal to 2, 3, 4, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + 3*a(n-3) - 2*a(n-4) -18*a(n-5)
 *            -29*a(n-6) -32*a(n-7) + 74*a(n-8) + 74*a(n-9) -90*a(n-10)
 *            -73*a(n-11) +33*a(n-12) -21*a(n-13) -29*a(n-14) +32*a(n-15)
 *            +4*a(n-16) +4*a(n-17) +8*a(n-18); Initial terms: a(1)=0, a(2)=2,
 *            a(3)=4, a(4)=13, a(5)=63, a(6)=264, a(7)=1005, a(8)=41...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array storing computed terms */
static size_t cache_size  = 0;       /* allocated capacity           */
static size_t computed    = 0;       /* number of terms actually filled */

static const int coeff[18] = {
    3,  4,  3, -2, -18, -29, -32, 74, 74, -90, -73, 33,
   -21, -29, 32,  4,  4,  8
};

static const seq_t init_vals[18] = {
    (seq_t)0, (seq_t)2, (seq_t)4, (seq_t)13, (seq_t)63,
    (seq_t)264, (seq_t)1005, (seq_t)4113, (seq_t)16720,
    (seq_t)67541, (seq_t)273173, (seq_t)1105028,
    (seq_t)4470455, (seq_t)18086203, (seq_t)73164414,
    (seq_t)295979201, (seq_t)1197372779, (seq_t)4843899390
};

/* Expand the cache to hold at least n elements. */
static void ensure_capacity(idx_t n)
{
    if (cache_size >= (size_t)n) return;
    size_t new_size = n > 64 ? (size_t)n : 64;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* In production code, handle the error. */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    /* Initialise new slots to zero. */
    for (size_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1 */

    /* Initialise cache with the first 18 terms on first call. */
    if (cache == NULL) {
        ensure_capacity(64);
        for (size_t i = 0; i < 18; ++i) cache[i] = init_vals[i];
        computed = 18;
    }

    /* If the requested term is already computed, return it. */
    if ((idx_t)computed >= n) return cache[n-1];

    /* Ensure the cache is large enough. */
    ensure_capacity(n);

    /* Compute missing terms using the recurrence. */
    for (idx_t i = computed; i < n; ++i) {
        seq_t sum = (seq_t)0;
        /* i is zero‑based: cache[i] holds a(i+1).  We need a(i-9)..a(i-1). */
        for (int k = 0; k < 18; ++k) {
            seq_t term = cache[i - 1 - k];
            if (coeff[k] > 0)
                sum += (seq_t)coeff[k] * term;
            else
                sum -= (seq_t)(-coeff[k]) * term;
        }
        cache[i] = sum;
    }
    computed = n;
    return cache[n-1];
}
