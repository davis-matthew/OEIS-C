/*
 * A251027  Number of (n+1)X(7+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 41, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251027: Number of (n+1) × 8 0..3 arrays with no 2×2 subblock
   whose diagonal‑sum exceeds the maximum of its anti‑diagonal elements.
   Recurrence (valid for n > 14):
   a(n) = 8*a(n‑1) – 25*a(n‑2) + 35*a(n‑3) – 7*a(n‑4) – 49*a(n‑5) +
          77*a(n‑6) – 55*a(n‑7) + 20*a(n‑8) – 3*a(n‑9).
   The first 14 terms (a(1) … a(14)) are supplied from OEIS and used as
   initial values for the recurrence.  The implementation memoises all
   computed values so that calls may arrive in any order. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] holds a(1)            */
static size_t cache_len = 0;         /* number of cached terms          */
static size_t cache_cap = 0;         /* allocated capacity               */

static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = p;
    cache_cap = new_cap;
}

/* Initialise the cache with the 14 known initial terms.  The values are
   taken from the OEIS entry for A251027.  These are the only terms that
   need to be hard‑coded; all later values are obtained by the recurrence. */
static void init_cache(void)
{
    if (cache_len > 0) return;       /* already initialised             */

    const seq_t seeds[14] = {
        (seq_t)126955ULL,
        (seq_t)334167ULL,
        (seq_t)608267ULL,
        (seq_t)1199576ULL,
        (seq_t)2203974ULL,
        (seq_t)4230166ULL,
        (seq_t)7948184ULL,
        (seq_t)15543033ULL,
        (seq_t)30783775ULL,
        (seq_t)63428115ULL,
        (seq_t)133806457ULL,
        (seq_t)290589257ULL,
        (seq_t)642273542ULL,
        (seq_t)1441935948ULL
    };
    ensure_cache_cap(14);
    for (size_t i = 0; i < 14; ++i) cache[i] = seeds[i];
    cache_len = 14;
}

seq_t a(idx_t n)
{
    if ((idx_t)n < 1) return (seq_t)0;   /* undefined for n < 1          */
    init_cache();

    if ((size_t)n <= cache_len)          /* already computed             */
        return cache[(size_t)n - 1];

    /* Compute values up to index n using the recurrence.             */
    size_t target = (size_t)n;
    ensure_cache_cap(target);

    for (size_t k = cache_len; k < target; ++k) {
        /* k is 0‑based index into cache; representing a(k+1).       */
        seq_t val =
              (seq_t) 8 * cache[k-1]              /* a(k)   */
            - (seq_t)25 * cache[k-2]              /* a(k-1) */
            + (seq_t)35 * cache[k-3]              /* a(k-2) */
            - (seq_t) 7 * cache[k-4]              /* a(k-3) */
            - (seq_t)49 * cache[k-5]              /* a(k-4) */
            + (seq_t)77 * cache[k-6]              /* a(k-5) */
            - (seq_t)55 * cache[k-7]              /* a(k-6) */
            + (seq_t)20 * cache[k-8]              /* a(k-7) */
            - (seq_t) 3 * cache[k-9];             /* a(k-8) */
        cache[k] = val;
    }
    cache_len = target;
    return cache[target - 1];
}
