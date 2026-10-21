/*
 * A297667  Number of chordless cycles in the n-Moebius ladder.
 * https://oeis.org/A297667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 2*cos(n*Pi/3) + Lucas(n)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) values are defined for n >= 1 (offset = 1) */

#include <stdlib.h>

/* Static cache for Lucas numbers, indexed by n (Lucas(0) = 2, Lucas(1) = 1) */
static seq_t *lucas_cache = NULL;
static idx_t lucas_len   = 0;   /* number of entries currently stored */

/* Ensure Lucas numbers up to index n are computed and stored in lucas_cache */
static void ensure_lucas(idx_t n)
{
    if (lucas_len > n)
        return;                       /* already computed */

    idx_t new_len = n + 1;                          /* allocate up to n inclusive */
    seq_t *new_cache = (seq_t *)realloc(lucas_cache, (size_t)new_len * sizeof(seq_t));
    if (!new_cache)
        exit(1);                                 /* allocation failure */

    /* initialise if this is the first allocation */
    if (lucas_len == 0) {
        new_cache[0] = (seq_t)2;                 /* Lucas(0) */
        new_cache[1] = (seq_t)1;                 /* Lucas(1) */
        lucas_len   = 2;
    } else if (lucas_len == 1) {                 /* we have Lucas(0) only */
        new_cache[1] = (seq_t)1;                 /* Lucas(1) */
        lucas_len   = 2;
    }

    /* extend the cache if necessary */
    while (lucas_len <= n) {
        new_cache[lucas_len] =
            new_cache[lucas_len - 1] + new_cache[lucas_len - 2];
        ++lucas_len;
    }

    /* if new_len > lucas_len, fill remaining entries (should not happen) */
    for (idx_t i = lucas_len; i <= n; ++i)
        new_cache[i] = new_cache[i - 1] + new_cache[i - 2];

    lucas_len   = new_len;
    lucas_cache = new_cache;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* defined only for n >= 1, but guard anyway */

    ensure_lucas(n);         /* guarantee Lucas(n) is ready */

    /* Compute the cosine term via its periodic integer values */
    idx_t mod = n % 6;
    int   cp;                /* -2, -1, 1, or 2 */
    switch (mod) {
        case 0: cp = -2; break;
        case 1: cp = -1; break;
        case 2: cp =  1; break;
        case 3: cp =  2; break;
        case 4: cp =  1; break;
        case 5: cp = -1; break;
        default: cp = 0;  /* never reached */
    }

    /* a(n) = n + cp + Lucas(n)  (cp may be negative) */
    seq_t result = (seq_t)n + lucas_cache[n];
    if (cp < 0)
        result -= (seq_t)(-cp);   /* subtract |-cp| */
    else
        result += (seq_t)cp;      /* add cp */

    return result;
}
