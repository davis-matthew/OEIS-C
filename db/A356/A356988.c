/*
 * A356988  a(n) = n - a^[2](n - a^[3](n-1)) with a(1) = 1, where a^[2](n) =
 *          a(a(n)) and a^[3](n) = a(a(a(n))).
 * https://oeis.org/A356988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - a²( n - a³(n-1) ), a(1)=1 Here a²(x) = a(a(x)), a³(x)
 *            = a(a(a(x))). Values are cached in an array indexed by n-1
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static memoisation table ------------------------------------------------- */
static seq_t *cache   = NULL;   /* cache[i] holds a(i+1) */
static size_t cap     = 0;      /* allocated entries */
static idx_t  sz      = 0;      /* number of entries currently stored */

/* Ensure that the cache can hold at least 'needed' entries. */
static void ensure_cap(size_t needed)
{
    if (cap >= needed) return;
    size_t newCap = cap ? cap << 1 : 1;
    while (newCap < needed) newCap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    if (!tmp) exit(1);                  /* unrecoverable allocation failure */
    cache = tmp;
    cap   = newCap;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* for completeness, not used by OEIS */

    if (n == 1)                         /* a(1) is defined explicitly */
        return 1;

    /* Return cached value if already computed. */
    if (n <= sz) return cache[n - 1];

    /* Allocate space for missing values. */
    ensure_cap((size_t)n);

    /* Compute missing values up to n. */
    for (idx_t i = sz + 1; i <= n; ++i)
    {
        if (i == 1)                     /* handled above, but kept for completeness */
        {
            cache[0] = 1;
            continue;
        }

        /* a(i-1) */
        seq_t a_prev = cache[i - 2];                /* i-2 >= 0 */

        /* a²(i-1) = a(a(i-1)) */
        seq_t a2_prev = cache[(idx_t)a_prev - 1];

        /* a³(i-1) = a(a(a(i-1))) */
        seq_t a3_prev = cache[(idx_t)a2_prev - 1];

        /* m = i - a³(i-1) */
        seq_t m = (seq_t)i - a3_prev;               /* m >= 1 */

        idx_t idxM = (idx_t)m;                      /* index of m in cache */

        /* a(m) */
        seq_t a_m = cache[idxM - 1];

        /* a²(m) = a(a(m)) */
        seq_t a2_m = cache[(idx_t)a_m - 1];

        /* a(i) = i - a²(m) */
        seq_t val = (seq_t)i - a2_m;

        cache[i - 1] = val;
    }

    sz = n;
    return cache[n - 1];
}
