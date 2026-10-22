/*
 * A365874  The "quadruple commas" sequence, a variant of A121805. See the
 *          Comments and Example sections for detailed explanations.
 * https://oeis.org/A365874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, changed
 * Formula:   a(1) = 1, and for n > 1 the next term a(n) is the smallest
 *            integer a(n-1) < a(n) satisfying a(n) – a(n-1) = 4 * (10 *
 *            (a(n-1) mod 10) + first_digit(a(n))), where "first_digit" is the
 *            most significant decimal digit of a(n)
 * Range:     u8 n <= 2, u16 n <= 285, u32 n <= 9694, u64 n <= 9694,
 *            u128 n <= 9694, big any n
 * Helpers:   digit_len, ensure_capacity, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/ensure_capacity.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Find the most significant decimal digit of an arbitrary-precision integer. */
static int first_digit(seq_t x)
{
    int len = digit_len(x);          /* number of decimal digits */
    seq_t base = pow10(len - 1);     /* 10^(len-1) */
    seq_t high = x / base;           /* truncating division */
    return (int)high;                /* high must be 0..9 */
}

/* Cache for computed terms. */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t  cache_len  = 0;   /* number of terms stored */

/* Make sure terms up to idx n are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Allocate space for n terms. */
    ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Compute missing terms. */
    for (idx_t k = cache_len + 1; k <= n; ++k)
    {
        if (k == 1)            /* a(1) = 1 */
        {
            cache[0] = 1;
            continue;
        }

        seq_t prev = cache[k - 2];           /* a(k-1) */
        seq_t last_digit = prev % 10;        /* L */

        seq_t best_candidate = 0;            /* 0 means not found yet */
        bool found = false;

        for (int first = 1; first <= 9; ++first)
        {
            seq_t m   = 10 * last_digit + first;           /* (10L + F) */
            seq_t diff = 4 * m;                            /* 4*(10L+F) */
            seq_t cand = prev + diff;                      /* a(k) candidate */

            if (first_digit(cand) == first)                /* satisfies rule */
            {
                if (!found || cand < best_candidate)
                {
                    best_candidate = cand;
                    found = true;
                }
            }
        }

        /* If no candidate was found, the sequence has ended. */
        if (!found)
        {
            cache_len = k - 1;
            return;
        }

        cache[k - 1] = best_candidate;
    }

    cache_len = n;
}

/* Public function as required. */
seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined, but just return 0 */
        return 0;

    ensure_cache(n);
    return cache[n - 1];
}
