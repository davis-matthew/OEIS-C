/*
 * A358734  Number of down-steps (1,-1) among all n-length nondecreasing Dyck
 *          paths with air pockets.
 * https://oeis.org/A358734
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A358734
 * Number of down-steps (1,-1) among all n‑length nondecreasing Dyck paths with air pockets.
 *
 * Recurrence (derived from the rational generating function):
 *
 *   a(0) = 0
 *   a(1) = 0
 *   a(2) = 1
 *   a(3) = 0
 *
 *   For n ≥ 4:
 *       a(n) = 5·a(n‑1) – 7·a(n‑2) + 4·a(n‑4) + p(n)
 *
 *   where p(n) is the coefficient of xⁿ in
 *
 *         x² – 5x³ + 9x⁴ – 7x⁵ + 2x⁶ + x⁷ – x⁸
 *
 *   i.e.  p(2)= 1 , p(3)=–5 , p(4)= 9 , p(5)=–7 , p(6)= 2 ,
 *         p(7)= 1 , p(8)=–1 , and p(n)=0 for n>8.
 *
 * The implementation stores previously computed values in a static cache
 * and extends it as required.  It is safe for 64‑bit and arbitrary‑precision
 * builds.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;      /* a[0] … a[cache_len‑1] */
static size_t cache_len = 0;     /* number of stored entries */

/* p(n) for 0 ≤ n ≤ 8; zero elsewhere */
static const int p_vals[9] = {0, 0, 1, -5, 9, -7, 2, 1, -1};

static void
ensure_cache(idx_t n)
{
    /* We need to store up to index n (inclusive) */
    size_t req = (size_t)n + 1;
    if (req <= cache_len)
        return;   /* already enough */

    /* Remember old length before we replace the array */
    size_t old_len = cache_len;

    /* Allocate a larger array that can hold the new indices */
    seq_t *new_cache = alloc_seq_array(req);
    if (new_cache == NULL)
        abort(); /* critical failure */

    /* Copy existing values */
    for (size_t i = 0; i < old_len; ++i)
        new_cache[i] = cache[i];

    /* Free the old cache, if any */
    if (cache != NULL)
        free_seq_array(cache);

    cache     = new_cache;
    cache_len = req;

    /* Initialise the first few values if necessary */
    if (cache_len > 0) cache[0] = 0;
    if (cache_len > 1) cache[1] = 0;
    if (cache_len > 2) cache[2] = 1;
    if (cache_len > 3) cache[3] = 0;

    /* Compute missing terms, starting from the first index that was not
     * already computed */
    size_t start = (old_len > 4) ? old_len : 4;
    for (size_t i = start; i <= (size_t)n; ++i)
    {
        seq_t term = (seq_t)5 * cache[i-1] + (seq_t)4 * cache[i-4];

        int pi = (i < 9 ? p_vals[i] : 0);
        if (pi >= 0)
            term += (seq_t)pi;
        else
            term -= (seq_t)(-pi);

        cache[i] = term - (seq_t)7 * cache[i-2];
    }
}

/* External linkage: the function required by the harness */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* behaviour for indices <2 is irrelevant */
    ensure_cache(n);
    return cache[(size_t)n];
}
