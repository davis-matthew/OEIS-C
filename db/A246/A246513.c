/*
 * A246513  a(n) = (4/n^2)*( Sum_{k=0..n-1} k*A246459(k) ).
 * https://oeis.org/A246513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246513
 * a(n) = (4/n^2) * Σ_{k=0}^{n-1} k * A246459(k)
 * Recurrence (n ≥ 3):
 *   (n-2)*n^2*(2n-7)*(4n-5)*a(n)
 *   = (n-1)*(80n^4 - 532n^3 + 1126n^2 - 893n + 195)*a(n-1)
 *     - 9*(n-2)^2*(n-1)*(2n-5)*(4n-1)*a(n-2)
 * Initial terms: a(1)=0, a(2)=7.
 *
 * The implementation caches computed values to allow arbitrary call
 * order.  The recurrence is used for all n≥3; the base values are
 * hard‑coded.  No external libraries are required other than the
 * utilities listed in the problem statement. */

#include <stdlib.h>

/* Cache of previously computed terms (1‑based, index 0 unused). */
static seq_t *cache   = NULL;   /* pointer to the cache array */
static size_t cap     = 0;       /* allocated number of slots (incl. 0) */
static idx_t  last    = 2;      /* highest index that has been computed */

/* Allocate or extend cache so that it can store indices up to `need`.
 * Newly allocated elements are zero‑initialised. */
static void ensure_cache(size_t need)
{
    if (need < cap)
        return;                 /* nothing to do */

    /* Allocate a new larger array (including slot 0). */
    size_t new_cap = need + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* out of memory – let program crash */

    /* Initialise all new slots to zero. */
    for (size_t i = cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cap   = new_cap;

    /* Initialise the known base terms (they stay the same even after a
     * reallocation).  Only initialise them if the cache is large enough. */
    if (cap > 1) cache[1] = 0;
    if (cap > 2) cache[2] = 7;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* undefined for n < 1, return 0 */

    /* Prepare cache to hold n. */
    ensure_cache((size_t)n);

    /* Return the value if it has already been computed. */
    if (n <= last)
        return cache[n];

    /* Compute terms from last+1 up to n. */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* Use different intermediate types depending on seq_t size. */
        if (sizeof(seq_t) <= 8)
        {
            /* For builtin integer types we use signed __int128
             * for the intermediate computations. */
            __int128_t ii = i;
            __int128_t n2 = ii * ii;
            __int128_t n3 = n2 * ii;
            __int128_t n4 = n3 * ii;

            __int128_t poly = 80 * n4 - 532 * n3 + 1126 * n2 - 893 * ii + 195;
            __int128_t left = (ii - 2) * ii * ii *
                              ((__int128_t)2 * ii - 7) *
                              ((__int128_t)4 * ii - 5);

            __int128_t term1 = (ii - 1) * poly *
                               (__int128_t)cache[i - 1];
            __int128_t term2 = 9 * (ii - 2) * (ii - 2) * (ii - 1) *
                               ((__int128_t)2 * ii - 5) *
                               ((__int128_t)4 * ii - 1) *
                               (__int128_t)cache[i - 2];
            __int128_t numer = term1 - term2;

            /* Integer division is exact for the recurrence. */
            cache[i] = (seq_t)(numer / left);
        }
        else
        {
            /* For arbitrary‑precision seq_t we rely on its built‑in
             * operators (C++17 or custom size). */
            seq_t nn  = (seq_t)i;
            seq_t n2  = nn * nn;
            seq_t n3  = n2 * nn;
            seq_t n4  = n3 * nn;

            seq_t poly = 80 * n4 - 532 * n3 + 1126 * n2 - 893 * nn + 195;
            seq_t left = (nn - 2) * nn * nn *
                         ((seq_t)2 * nn - 7) *
                         ((seq_t)4 * nn - 5);

            seq_t term1 = (nn - 1) * poly * cache[i - 1];
            seq_t term2 = 9 * (nn - 2) * (nn - 2) * (nn - 1) *
                         ((seq_t)2 * nn - 5) *
                         ((seq_t)4 * nn - 1) *
                         cache[i - 2];
            seq_t numer = term1 - term2;

            cache[i] = numer / left;
        }
    }

    last = n;
    return cache[n];
}
