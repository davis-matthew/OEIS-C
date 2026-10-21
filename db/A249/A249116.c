/*
 * A249116  Ordered union of the sets {h^6, h >=1} and {32*k^6, k >=1}.
 * https://oeis.org/A249116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ordered union of {h^6 | h≥1} and {32·k^6 | k≥1}.  The function
   a(idx_t n) returns the nth element (n≥1) of the sorted union.
   The routine builds a small cache and lazily merges the two
   monotone sequences using integer arithmetic only.  No external
   libraries are required. */

#include <stdlib.h>
#include <stdint.h>

/* ---- Helper: compute x^6 -------------------------------------------------- */
static inline seq_t pow6(seq_t x)
{
    seq_t x2 = x * x;       /* x^2  */
    seq_t x4 = x2 * x2;     /* x^4  */
    return x4 * x2;         /* x^6  */
}

/* ---- Static state for the memoised sequence ----------------------------- */
static seq_t *cache = NULL;      /* dynamic array of computed terms      */
static size_t cache_size = 0;    /* number of terms currently stored    */
static size_t cache_cap  = 0;    /* allocated capacity                   */
static uint64_t h = 1;           /* next index for h^6                  */
static uint64_t k = 1;           /* next index for 32*k^6                */
static seq_t nextS = 0;          /* current h^6 value                   */
static seq_t nextT = 0;          /* current 32*k^6 value                */
static int initialized = 0;      /* flag whether globals are initialised */

/* ---- Grow the cache array ------------------------------------------------ */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 64;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cache_cap = newcap;
}

/* ---- External sequence routine ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* below offset – not part of the sequence */

    /* Initialise globals on first call */
    if (!initialized) {
        h = 1;
        k = 1;
        nextS = pow6((seq_t)1);             /* 1^6 = 1          */
        nextT = (seq_t)32 * pow6((seq_t)1); /* 32·1^6 = 32     */
        initialized = 1;
    }

    /* Ensure the cache holds at least n terms. */
    grow_cache((size_t)n);
    while (cache_size < (size_t)n)
    {
        if (nextS <= nextT)
        {
            cache[cache_size++] = nextS;
            ++h;
            nextS = pow6((seq_t)h);
        }
        else
        {
            cache[cache_size++] = nextT;
            ++k;
            nextT = (seq_t)32 * pow6((seq_t)k);
        }
    }

    return cache[(size_t)n - 1];
}
