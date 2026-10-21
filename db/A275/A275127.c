/*
 * A275127  Number of nX4 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275127: Number of n×4 0..2 arrays with no element equal to any value
 * at offset (-2,-2) (-1,0) or (-1,1) and new values introduced in order 0..2.
 *
 * Recurrence (valid for n > 20):
 *   a(n) = 5*a(n-1) -9*a(n-2) +19*a(n-3) -9*a(n-4) -9*a(n-5)
 *          +87*a(n-6) -46*a(n-7) -84*a(n-8) +7*a(n-9)
 *          -59*a(n-10) -45*a(n-11) +31*a(n-12) +23*a(n-13)
 *          +4*a(n-14) - a(n-15) - a(n-16) + a(n-17)
 *
 * Initial values (n = 1 .. 23) are embedded in the table.
 *
 * The implementation caches computed terms so that calls may arrive in any order
 * and each term is computed only once.  The cache is grown with realloc, with
 * explicit casts to avoid the C++ conversion issue.
 */

#include <stdlib.h>

static const seq_t init_vals[23] = {
    14ULL, 64ULL, 174ULL, 568ULL, 2178ULL, 8321ULL, 31772ULL,
    121707ULL, 466252ULL, 1783920ULL, 6825273ULL, 26122839ULL,
    99981686ULL, 382636173ULL, 1464403303ULL, 5604594713ULL,
    21449747744ULL, 82091326930ULL, 314176968483ULL,
    1202409074708ULL, 4601819411125ULL, 17611921823170ULL,
    67403757335393ULL
};

static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), etc. */
static size_t cache_cap = 0;   /* allocated capacity (in terms) */
static idx_t  cache_size = 0;  /* highest index computed (+1) */

/* Ensure the cache can hold at least n terms (indices 0 .. n-1). */
static void ensure_cache(idx_t n)
{
    if (n <= (idx_t)cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 32 : cache_cap;
    while (new_cap < (size_t)n)
        new_cap <<= 1;

    cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Compute a(n) for n >= 1.  The function is memoised and works for calls
 * in any order. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 as a safe guard */
        return 0;

    if (n <= 23)
        return init_vals[n - 1];

    /* Load the initial values once when needed. */
    if (cache_size < 23) {
        ensure_cache(23);
        for (idx_t i = 0; i < 23; ++i)
            cache[i] = init_vals[i];
        cache_size = 23;
    }

    /* Compute missing terms up to n. */
    if (cache_size < n) {
        ensure_cache(n);
        for (idx_t m = cache_size + 1; m <= n; ++m) {      /* m = current term number */
            /* Index in cache: cache[m-1] holds a(m) */
            idx_t idx = m - 1; /* array index for a(m) */

            /* Apply the recurrence (valid for m >= 21).  It is safe for
             * m > 20 because we pre-filled at least the first 23 terms. */
            seq_t val = 5 * cache[idx - 1]          /* a(m-1) */
                      - 9 * cache[idx - 2]          /* a(m-2) */
                      + 19 * cache[idx - 3]         /* a(m-3) */
                      - 9 * cache[idx - 4]          /* a(m-4) */
                      - 9 * cache[idx - 5]          /* a(m-5) */
                      + 87 * cache[idx - 6]         /* a(m-6) */
                      - 46 * cache[idx - 7]         /* a(m-7) */
                      - 84 * cache[idx - 8]         /* a(m-8) */
                      + 7 * cache[idx - 9]          /* a(m-9) */
                      - 59 * cache[idx - 10]        /* a(m-10) */
                      - 45 * cache[idx - 11]        /* a(m-11) */
                      + 31 * cache[idx - 12]        /* a(m-12) */
                      + 23 * cache[idx - 13]        /* a(m-13) */
                      + 4 * cache[idx - 14]         /* a(m-14) */
                      - cache[idx - 15]             /* a(m-15) */
                      - cache[idx - 16]             /* a(m-16) */
                      + cache[idx - 17];            /* a(m-17) */

            cache[idx] = val;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
