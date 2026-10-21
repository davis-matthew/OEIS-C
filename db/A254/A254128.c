/*
 * A254128  Number of binary strings of length n that begin with an odd-length
 *          palindrome.
 * https://oeis.org/A254128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254128: number of binary strings of length n that begin
 * with an odd-length palindrome (non‑trivial).  The recurrence used is
 *
 *   a(0) = a(1) = a(2) = 0
 *   a(2*n)   = 2 * a(2*n-1)                     (n > 0)
 *   a(2*n+1) = 2 * a(2*n) + 2^(n+1) - a(n+1)    (n > 0)
 *
 * The implementation is memoised: values are stored in a static
 * dynamically allocated array and only the missing terms are computed
 * when queried.  The code works for any allowed definition of seq_t
 * and idx_t.
 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation table                         */
static size_t cap   = 0;      /* allocated capacity (number of entries)   */
static size_t len   = 0;      /* number of terms already initialised      */

/* Initialise the cache with the three known zero terms. */
static void
init_cache(void)
{
    if (cache == NULL) {
        cap = 4;                 /* allocate a few slots initially   */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) {
            /* In a non‑interactive build we should abort on OOM. */
            abort();
        }
        cache[0] = cache[1] = cache[2] = (seq_t)0;
        len = 3;
    }
}

/* Return the n‑th term of sequence A254128. */
seq_t
a(idx_t n)
{
    init_cache();

    /* Grow the table if necessary.  Use doubling to keep reallocs
     * infrequent. */
    if ((size_t)n >= cap) {
        size_t newcap = cap ?: 1;
        while (newcap <= (size_t)n) {
            newcap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
        cap = newcap;
    }

    /* Compute all missing values up to n. */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        if (i <= 2) {                      /* a(0), a(1), a(2) */
            cache[i] = (seq_t)0;
        } else if ((i & (idx_t)1) == 0u) {  /* even index: a(2*n) */
            cache[i] = cache[i - 1] << 1;
        } else {                            /* odd index:   a(2*n+1) */
            idx_t half   = (i - 1u) / 2u;    /* n  in the formula */
            seq_t twoPow = ((seq_t)1) << (unsigned)(half + 1u); /* 2^(n+1) */
            cache[i] = (cache[i - 1] << 1) + twoPow - cache[half + 1u];
        }
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}
