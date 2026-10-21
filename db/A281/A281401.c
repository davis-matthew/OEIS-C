/*
 * A281401  Number of 2 X n 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 16*a(n-3) - a(n-4) + 30*a(n-5) +
 *            4*a(n-6) - 24*a(n-7) - 4*a(n-8) + 8*a(n-9) for n > 10 Initial
 *            values: a(1)=0, a(2)=0, a(3)=10, a(4)=52, a(5)=170, a(6)=484,
 *            a(7)=1296, a(8)=3308, a(9)=8222, a(10)=19996
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 52, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, realloc, exit */

/* The following types are defined by the build environment */
typedef uint64_t idx_t;  /* idx_t – unsigned integer for indices */

/* Static cache for memoisation */
static seq_t *CACHE = NULL;
static size_t CACHE_CAP = 0;
static size_t CACHE_LEN = 0;   /* Number of terms currently stored */

/* Initialise the cache with the first 10 terms.  This function is
   intentionally thread‑unsafe; the problem statement does not
   require thread safety. */
static void init_cache(void)
{
    if (CACHE_LEN != 0) return;      /* Already initialised */

    CACHE_CAP = 64;
    CACHE = (seq_t *)malloc(CACHE_CAP * sizeof(seq_t));
    if (!CACHE) exit(EXIT_FAILURE);

    /* 10 known terms of the sequence */
    const unsigned long long init[10] =
        { 0ULL, 0ULL, 10ULL, 52ULL, 170ULL,
          484ULL, 1296ULL, 3308ULL, 8222ULL, 19996ULL };

    for (size_t i = 0; i < 10; ++i)
        CACHE[i] = (seq_t)init[i];

    CACHE_LEN = 10;
}

/* Grow the cache to hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= CACHE_CAP) return;

    size_t new_cap = needed * 2 + 16;
    if (new_cap < needed) new_cap = (size_t)-1;   /* max size_t */

    seq_t *tmp = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);

    CACHE = tmp;
    CACHE_CAP = new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */
    init_cache();

    /* Return cached value if already computed */
    if ((size_t)n <= CACHE_LEN) return CACHE[n - 1];

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)CACHE_LEN + 1; i <= n; ++i)
    {
        size_t idx = (size_t)i;          /* convert to size_t for indexing */
        grow_cache(idx);

        /* Recurrence: indices in C array are 0‑based, so
           CACHE[j-1] holds a(j).                  */
        seq_t val = 4 * CACHE[idx - 2]          /* 4*a(i-1) */
                 + CACHE[idx - 3]                /* + a(i-2) */
                 - 16 * CACHE[idx - 4]           /* -16*a(i-3) */
                 - CACHE[idx - 5]                /* - a(i-4) */
                 + 30 * CACHE[idx - 6]           /* +30*a(i-5) */
                 + 4 * CACHE[idx - 7]            /* +4*a(i-6) */
                 - 24 * CACHE[idx - 8]           /* -24*a(i-7) */
                 - 4 * CACHE[idx - 9]            /* -4*a(i-8) */
                 + 8 * CACHE[idx - 10];          /* +8*a(i-9) */

        CACHE[idx - 1] = val;
        CACHE_LEN = idx;
    }

    return CACHE[n - 1];
}
