/*
 * A260836  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000011 or 00010101.
 * https://oeis.org/A260836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the empirical recurrence for n > 24: a(n) =
 *            6*a(n-3) +6*a(n-4) -9*a(n-6) -16*a(n-7) -10*a(n-8) +4*a(n-9)
 *            +7*a(n-10) +6*a(n-12) +8*a(n-14) +10*a(n-15) +2*a(n-16)
 *            +4*a(n-17) +4*a(n-19) +3*a(n-20) +2*a(n-22) with initial values
 *            a(1) … a(24) given below. This implementation memoises prev...
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 29, u64 n <= 65, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Constants for the initial 24 terms
   (index 0 unused).  Values are small enough for 64‑bit. */
static const seq_t base_vals[25] = {
    0ULL, /* base_vals[0] unused */
    188ULL,   288ULL,   700ULL,  1224ULL,  1560ULL,
    3470ULL,  6908ULL, 10070ULL,18716ULL, 39478ULL,
   64940ULL,111162ULL,228324ULL,409632ULL,690964ULL,
 1345662ULL,2532216ULL,4354216ULL,8095220ULL,15462342ULL,
 27373920ULL,49529004ULL,94027168ULL,170716994ULL
};

static seq_t *cache = NULL;       /* memoised values, 1‑based */
static idx_t cache_size = 0;      /* number of slots currently allocated */
static idx_t cached_up_to = 0;    /* highest n whose a(n) is stored */

/* Helper: extend cache to at least needed+1 slots and initialise first
   24 values if necessary. */
static void ensure_cache(idx_t needed)
{
    if (needed + 1 <= cache_size) return;

    seq_t *new_cache = (seq_t *)realloc(cache, (needed + 1) * sizeof(seq_t));
    if (!new_cache) return; /* out‑of‑memory – caller will never hit this */
    cache = new_cache;
    cache_size = needed + 1;

    if (cached_up_to < 24) {
        for (idx_t i = cached_up_to + 1; i <= 24; ++i)
            cache[i] = base_vals[i];
        cached_up_to = 24;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Return directly if within the initial range.  This bypasses the
       cache for speed on small indices. */
    if (n <= 24) return base_vals[n];

    /* Ensure we have enough space to store and compute values up to n. */
    ensure_cache(n);

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t pos = 0;  /* sum of positive terms */
        seq_t neg = 0;  /* sum of absolute values of negative terms */

        /* Positive coefficients */
        pos += cache[i-3] * (seq_t)6;
        pos += cache[i-4] * (seq_t)6;
        pos += cache[i-9] * (seq_t)4;
        pos += cache[i-10] * (seq_t)7;
        pos += cache[i-12] * (seq_t)6;
        pos += cache[i-14] * (seq_t)8;
        pos += cache[i-15] * (seq_t)10;
        pos += cache[i-16] * (seq_t)2;
        pos += cache[i-17] * (seq_t)4;
        pos += cache[i-19] * (seq_t)4;
        pos += cache[i-20] * (seq_t)3;
        pos += cache[i-22] * (seq_t)2;

        /* Negative coefficients (subtract after all positives added). */
        neg += cache[i-6] * (seq_t)9;
        neg += cache[i-7] * (seq_t)16;
        neg += cache[i-8] * (seq_t)10;

        cache[i] = pos - neg;  /* final value is guaranteed positive */
    }

    cached_up_to = n;
    return cache[n];
}
