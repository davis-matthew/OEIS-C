/*
 * A259005  Number of (n+2)X(7+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two medians of the central row and column plus the two sums of
 *          the diagonal and antidiagonal nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A259005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = a(n-1) + a(n-12) - a(n-13) for n > 25 * a(1) … a(24) as
 *            given in the OEIS entry. * a(25) is obtained from the empirical
 *            linear formula: * a(n) = 592*n + 54374, n mod 12 = 1, n > 12 *
 *            The sequence is computed lazily and memoised in static storage
 * Range:     u8 none, u16 none, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* 1‑based array of values */
static size_t cache_size = 0; /* length of allocated array */

/* Initialise the cache with the first 25 terms.  */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    /* indices 0..25  (index 0 unused) */
    cache = (seq_t *)malloc(26 * sizeof(seq_t));
    if (!cache)
        return; /* allocation failure: caller will get uninitialised value */

    /* a(1) .. a(24) from the OEIS data */
    cache[1]  = (seq_t) 914320;   /* a(1) */
    cache[2]  = (seq_t) 701282;   /* a(2) */
    cache[3]  = (seq_t)2005182;   /* a(3) */
    cache[4]  = (seq_t)1284465;   /* a(4) */
    cache[5]  = (seq_t) 431280;   /* a(5) */
    cache[6]  = (seq_t) 390714;   /* a(6) */
    cache[7]  = (seq_t)  73734;   /* a(7) */
    cache[8]  = (seq_t)  61001;   /* a(8) */
    cache[9]  = (seq_t)  59336;   /* a(9) */
    cache[10] = (seq_t)  60827;   /* a(10) */
    cache[11] = (seq_t)  60804;   /* a(11) */
    cache[12] = (seq_t)  61979;   /* a(12) */
    cache[13] = (seq_t)  62070;   /* a(13) */
    cache[14] = (seq_t)  63065;   /* a(14) */
    cache[15] = (seq_t)  63258;   /* a(15) */
    cache[16] = (seq_t)  63274;   /* a(16) */
    cache[17] = (seq_t)  64258;   /* a(17) */
    cache[18] = (seq_t)  64949;   /* a(18) */
    cache[19] = (seq_t)  65852;   /* a(19) */
    cache[20] = (seq_t)  66119;   /* a(20) */
    cache[21] = (seq_t)  65838;   /* a(21) */
    cache[22] = (seq_t)  67774;   /* a(22) */
    cache[23] = (seq_t)  67880;   /* a(23) */
    cache[24] = (seq_t)  69079;   /* a(24) */

    /* a(25) from the empirical linear formula for n mod 12 = 1 */
    cache[25] = (seq_t)(592ull * 25ull + 54374ull);

    cache_size = 26; /* indices 0..25 valid, index 0 unused */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 */

    init_cache();

    /* Resize cache if the needed index is beyond current size */
    if ((size_t)n >= cache_size) {
        size_t new_size = n + 1; /* 1-based index, include index n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation failure -> 0 */

        cache = new_cache;
        /* Compute missing values using recurrence */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* For i >= 26 recurrence applies */
            if (i >= 26) {
                cache[i] = cache[i-1] + cache[i-12] - cache[i-13];
            } else {
                /* For i <= 25 values already set */
                /* This branch is only reached if cache_size > 26,
                   which cannot happen here. */
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
