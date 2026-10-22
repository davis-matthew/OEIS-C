/*
 * A359991  Number of connected spanning subgraphs in the 4 X n grid graph.
 * https://oeis.org/A359991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 105 a(n-1) - 2907
 *            a(n-2) + 33255 a(n-3) - 183316 a(n-4) + 519800 a(n-5) - 778624
 *            a(n-6) + 610800 a(n-7) - 237312 a(n-8) + 39680 a(n-9) - 2048
 *            a(n-10) for n ≥ 11, with a(1)=1, a(2)=105, …,
 *            a(10)=63177423571626685
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[11] = {
    0ULL,                              /* unused: 0 */
    1ULL,                              /* a(1) */
    105ULL,                            /* a(2) */
    7857ULL,                           /* a(3) */
    555195ULL,                         /* a(4) */
    38757695ULL,                       /* a(5) */
    2698167665ULL,                     /* a(6) */
    187715481077ULL,                   /* a(7) */
    13057666054455ULL,                 /* a(8) */
    908271228919067ULL,                /* a(9) */
    63177423571626685ULL               /* a(10) */
};

static const seq_t coeff[11] = {
    0ULL,          /* unused: 0 */
    105ULL,        /* coeff[1] */
    2907ULL,       /* coeff[2] */
    33255ULL,      /* coeff[3] */
    183316ULL,     /* coeff[4] */
    519800ULL,     /* coeff[5] */
    778624ULL,     /* coeff[6] */
    610800ULL,     /* coeff[7] */
    237312ULL,     /* coeff[8] */
    39680ULL,      /* coeff[9] */
    2048ULL        /* coeff[10] */
};

static const int sign[11] = {
    0,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1
};

static seq_t *cache = NULL;   /* dynamic array storing computed values */
static size_t cache_cap = 0;  /* allocated capacity */
static idx_t  cache_size = 0; /* last index computed (>= 10 after init) */

/* Ensure that cache holds values up to index n */
static void ensure_cache(idx_t n)
{
    /* Grow array if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first ten terms on first use */
    if (cache_size == 0) {
        cache[0] = 0;                   /* unused */
        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = init[i];
        cache_size = 10;
    }

    /* Compute further terms using the recurrence */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (int m = 1; m <= 10; ++m) {
            seq_t term = coeff[m] * cache[k - m];
            if (sign[m] > 0)
                sum += term;
            else
                sum -= term;
        }
        cache[k] = sum;
    }
    cache_size = n;
}

/* OEIS interface function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */
    if (n <= 10) return init[n];      /* initial known terms */
    ensure_cache(n);                  /* compute up to n if needed */
    return cache[n];
}
