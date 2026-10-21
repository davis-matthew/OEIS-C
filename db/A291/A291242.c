/*
 * A291242  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - 2 S - S^2 + S^3.
 * https://oeis.org/A291242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 5*a(n-3) - 4*a(n-4) + 2*a(n-5) +
 *            a(n-6) for n >= 7. Initial values: a(0)=2, a(1)=5, a(2)=13,
 *            a(3)=35, a(4)=91, a(5)=241, a(6)=631
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define MAX_COMPUTED 20000    /* enough for all test cases (n up to ~10 000) */

static seq_t cache[MAX_COMPUTED + 1];
static size_t cached_len = 0;   /* number of terms stored in cache (>=1) */

/* compute A291242 number a(n) */
seq_t a(idx_t n)
{
    /* initialise cache with the first seven values on first call */
    if (cached_len == 0) {
        cache[0] = 2;
        cache[1] = 5;
        cache[2] = 13;
        cache[3] = 35;
        cache[4] = 91;
        cache[5] = 241;
        cache[6] = 631;
        cached_len = 7;
    }

    /* if requested index is already cached, return it */
    if ((size_t)n < cached_len) {
        return cache[n];
    }

    /* fill the cache up to requested n *or* up to MAX_COMPUTED
       whichever comes first */
    size_t limit = (size_t)n;
    if (limit > MAX_COMPUTED) {
        limit = MAX_COMPUTED;
    }

    for (size_t i = cached_len; i <= limit; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];

        cache[i] = 2 * a1 + 4 * a2 - 5 * a3 - 4 * a4 + 2 * a5 + a6;
    }
    cached_len = limit + 1;          /* cache now up to 'limit' */

    /* if the requested index is within the cached range, return it */
    if ((size_t)n <= limit) {
        return cache[n];
    }

    /* n > MAX_COMPUTED: compute the term on the fly using a ring buffer
       of the last six cached values.  The ring buffer is indexed as
       ring[0] = a(i-6), ring[1] = a(i-5), ..., ring[5] = a(i-1). */
    seq_t ring[6];
    ring[5] = cache[MAX_COMPUTED];      /* a(M-1) */
    ring[4] = cache[MAX_COMPUTED - 1];  /* a(M-2) */
    ring[3] = cache[MAX_COMPUTED - 2];  /* a(M-3) */
    ring[2] = cache[MAX_COMPUTED - 3];  /* a(M-4) */
    ring[1] = cache[MAX_COMPUTED - 4];  /* a(M-5) */
    ring[0] = cache[MAX_COMPUTED - 5];  /* a(M-6) */

    seq_t result = ring[5];   /* placeholder for current term */

    for (idx_t i = (idx_t)MAX_COMPUTED + 1; i <= n; ++i) {
        result = 2 * ring[5] + 4 * ring[4] - 5 * ring[3] - 4 * ring[2] + 2 * ring[1] + ring[0];

        /* shift ring left by one position, discarding the oldest term */
        ring[0] = ring[1];
        ring[1] = ring[2];
        ring[2] = ring[3];
        ring[3] = ring[4];
        ring[4] = ring[5];
        ring[5] = result;
    }
    return result;
}
