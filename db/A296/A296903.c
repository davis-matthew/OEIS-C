/*
 * A296903  Numbers whose base-20 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          = #(peaks); see Comments.
 * https://oeis.org/A296903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th integer whose base‑20 digits have the same
 *            number of pits and peaks. The function finds terms by testing
 *            consecutive integers and caching the qualifying values
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: Works with every supported seq_t (built‑in unsigned
 * integers or arbitrary‑precision integer class) as long as
 * the usual arithmetic operators (% / +) are defined for
 * seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------- */
/* Helper: count pits and peaks in the base‑20 representation of x.  */
/* A pit is a digit d such that d(i-1) > d < d(i+1),
 * a peak if d(i-1) < d > d(i+1).  Digits at the ends cannot be
 * pits or peaks.  The digits are extracted least‑significant
 * first.                                                */
static void count_pits_peaks(seq_t x, int *pits, int *peaks)
{
    /* 20‑digit values fit in a byte.  1024 is ample for any
     * 128‑bit integer, and safe for arbitrary precision
     * objects that fit into RAM. */
    uint8_t  dig[1024];
    size_t   len = 0;
    seq_t    v   = x;

    /* Extract digits.  For x == 0 the loop runs once. */
    do {
        dig[len++] = (uint8_t)(v % 20);
        v /= 20;
    } while (v);

    /* Count pits and peaks. */
    int p = 0, q = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        if (dig[i - 1] > dig[i] && dig[i] < dig[i + 1])
            ++p;
        else if (dig[i - 1] < dig[i] && dig[i] > dig[i + 1])
            ++q;
    }
    *pits  = p;
    *peaks = q;
}

/* ------------------------------------------------------------------- */
/* Cache --------------------------------------------------------------- */
static seq_t *cache   = NULL;   /* array of discovered terms      */
static idx_t  cache_len = 0;    /* number of cached terms         */
static idx_t  tested    = 0;    /* largest integer that has been tested */
static size_t cache_cap = 0;    /* allocated capacity of the cache */

/* Grow the cache so that it contains at least target terms. */
static void ensure_cache(idx_t target)
{
    while (cache_len < target) {
        /* Grow the storage if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap) ? cache_cap * 2 : 1024;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* allocation failure */
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* Test the next integer. */
        seq_t v = tested + (seq_t)1;
        int pits, peaks;
        count_pits_peaks(v, &pits, &peaks);
        if (pits == peaks)
            cache[cache_len++] = v;   /* store qualifying number */
        ++tested;
    }
}

/* ------------------------------------------------------------------- */
/* Public API ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of bounds; undefined in OEIS */
    ensure_cache(n);                /* guarantee at least n terms are cached */
    return cache[n - 1];
}
