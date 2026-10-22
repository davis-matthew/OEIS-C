/*
 * A376467  Triangular array read by rows: A063007 * A007318.
 * https://oeis.org/A376467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A376467 – triangular array read by rows
 *
 *  T(n,k) satisfies 
 *      (n-k)·T(n,k) = 3·(2n-1)·T(n-1,k) – (n+k-1)·T(n-2,k)    (n≥2)
 *      T(n,n) = binom(2n,n)
 *  a(n) is the n-th entry when the triangle is read row‑wise.
 *
 *  The implementation builds successive rows on demand, caching all
 *  computed entries in a flat array.  The recurrence uses only the two
 *  previous rows; once a row is no longer needed its memory is freed.
 *  The maximum row that has been constructed is kept in 'max_row', so
 *  any call to a(n) instantly computes all rows up to the required one.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binom.h"       /* provides seq_t binom(idx_t a, idx_t b) */

/* integer square root of an unsigned 64‑bit integer (floor) */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, r = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            r = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return r;
}

/* fixed‑size cache for all triangle entries computed so far */
static seq_t  *cache      = NULL;   /* flat array of values */
static size_t  cache_size = 0;      /* number of elements stored */
static size_t  cache_cap  = 0;      /* allocated capacity */

/* two most recent rows – used to evaluate the recurrence */
static seq_t *prev      = NULL;     /* row n‑1, length prev_len   */
static size_t prev_len  = 0;
static seq_t *prevprev  = NULL;     /* row n‑2, length prevprev_len */
static size_t prevprev_len = 0;

/* highest row index that has been fully computed (0‑based) */
static uint64_t max_row = 0;

/* whether the static storage has been initialised */
static bool initialized = false;

/* enlarge the flat cache so it can hold at least 'need' additional
 * entries.  The new capacity is a power of two so reallocation is
 * infrequent. */
static void grow_cache(size_t need)
{
    if (cache_size + need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < cache_size + need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();           /* out of memory – hard fail */
    cache = tmp;
    cache_cap = new_cap;
}

/* one‑time initialisation: row 0 only and the empty cache */
static void init_static(void)
{
    /* row 0: T(0,0)=1 */
    prev = (seq_t *)malloc(sizeof(seq_t));
    if (!prev)
        abort();
    prev[0] = 1;
    prev_len = 1;

    /* no previous‑previous row yet */
    prevprev      = NULL;
    prevprev_len  = 0;
    max_row       = 0;

    /* initialise cache with the single value */
    cache_size = 0;
    cache_cap  = 0;
    grow_cache(1);
    cache[0] = 1;
    cache_size = 1;

    initialized = true;
}

/* construct all rows up to 'need_row' (inclusive).  New rows are
 * appended to the flat cache. */
static void compute_up_to(uint64_t need_row)
{
    while (max_row < need_row) {
        uint64_t n = max_row + 1;          /* compute row n */

        seq_t *new_row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_row)
            abort();

        if (n == 1) {                     /* explicit base row */
            new_row[0] = 3;
            new_row[1] = 2;
        } else {
            /* recurrence for k = 0 .. n-1 */
            for (uint64_t k = 0; k <= n - 1; ++k) {
                /* t1 = 3*(2n-1)*T(n-1,k) */
                seq_t t1 = prev[k] * ((seq_t)3 * ((seq_t)2 * n - 1));

                /* t2 = (n+k-1)*T(n-2,k)  if such a term exists */
                seq_t t2 = 0;
                if (k < prevprev_len)
                    t2 = prevprev[k] * ((seq_t)(n + k - 1));

                /* numerator and division */
                seq_t numerator = t1 - t2;
                seq_t denominator = (seq_t)(n - k);
                new_row[k] = numerator / denominator;
            }
            /* last element T(n,n) = binom(2*n,n) */
            new_row[n] = binom((idx_t)(2 * n), (idx_t)n);
        }

        /* append the new row to the cache */
        grow_cache(n + 1);
        for (size_t i = 0; i < n + 1; ++i)
            cache[cache_size++] = new_row[i];

        /* shift row pointers: prevprev ← prev, prev ← new_row */
        if (prevprev)
            free(prevprev);
        prevprev      = prev;
        prevprev_len  = prev_len;
        prev          = new_row;
        prev_len      = n + 1;

        max_row = n;
    }
}

/* ----------------------------------------------------------
 *  public sequence function
 * ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init_static();

    /* row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t disc = 8 * n + 1;
    idx_t sq   = isqrt_u64(disc);
    idx_t r    = (sq - 1) / 2;

    /* column index within the row */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k         = n - row_start;

    /* compute rows up to r if they aren't ready yet */
    if ((uint64_t)r > max_row)
        compute_up_to((uint64_t)r);

    /* final value is at position row_start + k in the flat cache */
    return cache[(size_t)(row_start + k)];
}
