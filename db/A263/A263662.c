/*
 * A263662  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -4 to 4 places and with no two consecutive
 *          increases or two consecutive decreases.
 * https://oeis.org/A263662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A263662
 *
 *  Formula:
 *      a(1)=1, a(2)=2, a(3)=4, a(4)=10, a(5)=32, a(6)=95, a(7)=208,
 *      a(8)=486, a(9)=1064, a(10)=2560, a(11)=6000, a(12)=14636,
 *      a(13)=33680, a(14)=81608, a(15)=187488, a(16)=454456,
 *      a(17)=1046368, a(18)=2537664, a(19)=5840768, a(20)=14162704
 *          (values for 1 ≤ n ≤ 20)
 *
 *      a(n) = 4*a(n-2) + 6*a(n-4) + 16*a(n-6) – 8*a(n-10)   (n > 20)
 *
 *  The function keeps a static cache so that calls in any order are
 *  fast.  The cache grows only as needed.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;      /* dynamic array of computed terms   */
static idx_t  cache_size = 0;    /* number of allocated elements      */

/* Base values up to n = 20 */
static const seq_t base_values[21] = {
    0,          /* dummy for index 0 (unused) */
    1,          /* a(1)  */
    2,          /* a(2)  */
    4,          /* a(3)  */
    10,         /* a(4)  */
    32,         /* a(5)  */
    95,         /* a(6)  */
    208,        /* a(7)  */
    486,        /* a(8)  */
    1064,       /* a(9)  */
    2560,       /* a(10) */
    6000,       /* a(11) */
    14636,      /* a(12) */
    33680,      /* a(13) */
    81608,      /* a(14) */
    187488,     /* a(15) */
    454456,     /* a(16) */
    1046368,    /* a(17) */
    2537664,    /* a(18) */
    5840768,    /* a(19) */
    14162704    /* a(20) */
};

/* Ensure the cache can hold indices up to n, compute new entries if necessary */
static void ensure_cache(idx_t n)
{
    /* initialise on first use */
    if (cache == NULL)
    {
        size_t init_cap = 21;                 /* enough for 0..20 */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return;           /* allocation failure – value undefined */

        /* copy known base values */
        for (idx_t i = 1; i <= 20; ++i)
            cache[i] = base_values[i];

        cache_size = init_cap;
    }

    if (cache_size <= n)                     /* need to grow */
    {
        size_t new_cap = (size_t)(n + 1);     /* indices 0..n inclusive */

        /* enlarge the allocated array */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return;       /* allocation failure – value undefined */

        /* compute values beyond the previous size up to n */
        idx_t start = (cache_size > 20 ? cache_size : 21);
        for (idx_t i = start; i <= n; ++i)
        {
            new_cache[i] = (seq_t)4 * new_cache[i - 2]
                          + (seq_t)6 * new_cache[i - 4]
                          + (seq_t)16 * new_cache[i - 6]
                          - (seq_t)8 * new_cache[i - 10];
        }

        cache = new_cache;
        cache_size = (idx_t)new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – offset is 1 */

    ensure_cache(n);               /* compute / make sure n is available */

    return cache[n];
}
