/*
 * A259366  Numbers for which the differences between consecutive divisors
 *          (ordered by size) are not distinct.
 * https://oeis.org/A259366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by incremental search for integers whose consecutive divisor
   gaps are not all distinct. */
   
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Comparator for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Return true if n has at least one repeated gap between consecutive
   divisors (divisors are sorted ascending). */
static bool has_duplicate_gap(uint64_t n)
{
    if (n < 4)                 /* fewer than two gaps */
        return false;

    /* Collect divisors into a small array */
    uint64_t divs[2048];
    int dcnt = 0;
    for (uint64_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            divs[dcnt++] = d;
            if (d != n / d)
                divs[dcnt++] = n / d;
        }
    }

    if (dcnt <= 2)             /* only one gap */
        return false;

    /* Sort divisors */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* Compute gaps */
    uint64_t gaps[2048];
    int gcnt = dcnt - 1;
    for (int i = 1; i < dcnt; ++i)
        gaps[i - 1] = divs[i] - divs[i - 1];

    /* Sort gaps and look for a duplicate */
    qsort(gaps, gcnt, sizeof(uint64_t), cmp_uint64);
    for (int i = 1; i < gcnt; ++i)
        if (gaps[i] == gaps[i - 1])
            return true;

    return false;
}

/* Cache of previously found sequence terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;       /* start checking from 1 */

/* Ensure that cache has at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        /* Find the next integer with a duplicate divisor gap. */
        for (;;)
        {
            if (has_duplicate_gap((uint64_t)next_candidate))
            {
                /* Grow cache if necessary. */
                if (cache_len >= cache_cap)
                {
                    size_t new_cap = (cache_cap ? cache_cap * 2 : 8);
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }

                cache[cache_len++] = (seq_t)next_candidate;
                ++next_candidate;   /* move to the next candidate */
                break;             /* found one */
            }
            ++next_candidate;
        }
    }
}

/* Public sequence function.  External linkage as requested. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
