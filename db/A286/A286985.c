/*
 * A286985  Number of connected dominating sets in the n-prism graph.
 * https://oeis.org/A286985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A286985: Number of connected dominating sets in the n-prism graph
   Recurrence for n > 6:
       a(n) = 6*a(n-1) - 11*a(n-2) + 4*a(n-3) + 5*a(n-4) - 2*a(n-5) - a(n-6)
   Initial values:
       a(1) = 7; a(2) = 7; a(3) = 39; a(4) = 115; a(5) = 343; a(6) = 967
*/

#include <stddef.h>

static seq_t cache[10001] = { 0 };       /* static array; element 0 unused */
static idx_t cache_size = 0;             /* highest index computed so far */

/* Ensure that cache[1..n] are computed.  n is assumed to be <= 10000. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Initialize the first six values (if not already done). */
    if (cache_size < 6) {
        cache[1] = 7;
        cache[2] = 7;
        cache[3] = 39;
        cache[4] = 115;
        cache[5] = 343;
        cache[6] = 967;
    }

    /* Compute values from max(cache_size+1, 7) up to n. */
    idx_t start = cache_size + 1;
    if (start < 7)
        start = 7;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = 6 * cache[i-1]
                 - 11 * cache[i-2]
                 + 4 * cache[i-3]
                 + 5 * cache[i-4]
                 - 2 * cache[i-5]
                 - cache[i-6];
    }

    /* Update the recorded maximum index. */
    if (n > cache_size)
        cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */
    if (n > 10000)
        abort();                       /* out of supported range */

    ensure_cache(n);
    return cache[n];
}
