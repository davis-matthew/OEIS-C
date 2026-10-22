/*
 * A309567  a(1) = 4, a(2) = 2, a(3) = 5, a(4) = 3, a(5) = 1; a(n) =
 *          a(n-a(n-1)) + a(n-a(n-4)) for n > 5.
 * https://oeis.org/A309567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4, a(2)=2, a(3)=5, a(4)=3, a(5)=1; *
 *            a(n)=a(n-a(n-1))+a(n-a(n-4)) for n>5 (OEIS A309567)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* seq_t and idx_t are supplied by the build environment.
   No other types are used. */

static seq_t cache[10010] = {0};   /* enough for all tests up to n≈10000 */
static size_t cache_upto = 0;      /* greatest index that has been filled */

/* Helper: initialise the first five values at most once. */
static void init_first_five(void)
{
    if (cache[1] == 0) {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)1;
        cache_upto = 5;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;          /* undefined, but safe */

    init_first_five();                         /* ensure base cases exist */

    /* If requested n is larger than the cache, extend it. */
    if ((size_t)n > cache_upto) {
        for (size_t i = cache_upto + 1; i <= (size_t)n; ++i) {
            /* Offsets are known to be positive and < i. */
            idx_t off1 = (idx_t)cache[i - 1];
            idx_t off4 = (idx_t)cache[i - 4];
            size_t pos1 = i - (size_t)off1;
            size_t pos4 = i - (size_t)off4;
            cache[i] = cache[pos1] + cache[pos4];
        }
        cache_upto = n;
    }

    return cache[(size_t)n];
}
