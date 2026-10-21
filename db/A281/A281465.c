/*
 * A281465  Number of n X 4 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281465:
   a(n) = a(n-1) + a(n-2) + 4*a(n-3) + a(n-4)
          - 4*a(n-5) - 5*a(n-6) + 3*a(n-8)   for n > 10
   Initial values:
     a(1)=8,  a(2)=37,  a(3)=39,  a(4)=96,  a(5)=207,
     a(6)=405, a(7)=897, a(8)=1975, a(9)=4154, a(10)=8884 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.
       cache[i] holds a(i+1).  cache_size = number of terms stored. */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* count of cached terms      */
    static size_t cache_cap  = 0;   /* allocated capacity         */

    /* Base values for n = 1..10.  We store them in the cache directly. */
    static const seq_t base[10] = {
        8ULL,    /* a(1) */
        37ULL,   /* a(2) */
        39ULL,   /* a(3) */
        96ULL,   /* a(4) */
        207ULL,  /* a(5) */
        405ULL,  /* a(6) */
        897ULL,  /* a(7) */
        1975ULL, /* a(8) */
        4154ULL, /* a(9) */
        8884ULL  /* a(10) */
    };

    /* Initialise the cache on first call. */
    if (cache_size == 0) {
        cache_cap = 16;                      /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = base[i];
        cache_size = 10;                     /* we now have 10 terms stored */
    }

    /* Return immediately if the requested term is already cached. */
    if (n <= cache_size)
        return cache[n - 1];                  /* cache is 0‑based */

    /* Ensure the cache is large enough to hold the required terms. */
    if (n > cache_cap) {
        /* Double until we have enough space (or just grow to n). */
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size; i < n; ++i) {
        /* i is 0‑based index for a(i+1) */
        idx_t k = i + 1;                      /* current n in 1‑based form */
        seq_t a_nm1 = cache[k - 2];           /* a(n-1) */
        seq_t a_nm2 = cache[k - 3];           /* a(n-2) */
        seq_t a_nm3 = cache[k - 4];           /* a(n-3) */
        seq_t a_nm4 = cache[k - 5];           /* a(n-4) */
        seq_t a_nm5 = cache[k - 6];           /* a(n-5) */
        seq_t a_nm6 = cache[k - 7];           /* a(n-6) */
        seq_t a_nm8 = cache[k - 9];           /* a(n-8) */

        /* Apply the empirical recurrence. */
        seq_t term =  a_nm1 + a_nm2
                   + (seq_t)4 * a_nm3
                   + a_nm4
                   - (seq_t)4 * a_nm5
                   - (seq_t)5 * a_nm6
                   + (seq_t)3 * a_nm8;
        cache[i] = term;
    }

    /* Update the number of stored terms. */
    cache_size = n;

    return cache[n - 1];
}
