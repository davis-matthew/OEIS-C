/*
 * A281028  Number of n X 2 0..1 arrays with no element equal to more than
 *          three of its king-move neighbors, with the exception of exactly
 *          one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 2*a(n-3) - 17*a(n-4) + 24*a(n-6) +
 *            16*a(n-7) + 12*a(n-8) - 16*a(n-9) - 16*a(n-10) Initial terms:
 *            a(1)=0, a(2)=0, a(3)=2, a(4)=12, a(5)=58, a(6)=252, a(7)=1048,
 *            a(8)=4204, a(9)=16454, a(10)=63236
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Static cache: index 0 is unused; we pre‑allocate enough space for
 * the typical maximum of ~10000 requested indices.  The array is
 * of type seq_t so it works with any provided integer or big‑int
 * type. */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE + 1];
static idx_t cache_len = 0;

/* Initialise the cache with the known first ten values. */
static void init_cache(void)
{
    if (cache_len > 0)      /* already initialised */
        return;

    cache[0] = (seq_t)0;     /* unused slot for convenience */

    cache[1] = (seq_t)0;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)2;
    cache[4] = (seq_t)12;
    cache[5] = (seq_t)58;
    cache[6] = (seq_t)252;
    cache[7] = (seq_t)1048;
    cache[8] = (seq_t)4204;
    cache[9] = (seq_t)16454;
    cache[10] = (seq_t)63236;

    cache_len = 10;
}

seq_t a(idx_t n)
{
    if (n < 1)               /* offset starts at 1 */
        return (seq_t)0;

    /* First call: set up the cache. */
    if (cache_len == 0)
        init_cache();

    /* Grow the cache if necessary. */
    if (n > cache_len) {
        /* We assume n will not exceed MAX_CACHE; for larger
         * values the function can be extended to reallocate. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* Compute the recurrence using only additions and
             * subtractions of positive terms.  All intermediate
             * values are non‑negative, so no unsigned underflow
             * can occur. */
            seq_t s_pos = (seq_t)6 * cache[i - 1];
            s_pos += (seq_t)24 * cache[i - 6];
            s_pos += (seq_t)16 * cache[i - 7];
            s_pos += (seq_t)12 * cache[i - 8];

            seq_t s_neg = (seq_t)7 * cache[i - 2];
            s_neg += (seq_t)2 * cache[i - 3];
            s_neg += (seq_t)17 * cache[i - 4];
            s_neg += (seq_t)16 * cache[i - 9];
            s_neg += (seq_t)16 * cache[i - 10];

            cache[i] = s_pos - s_neg;
        }
        cache_len = n;
    }

    return cache[n];
}
