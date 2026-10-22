/*
 * A318019  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A318019 (OEIS):
   a(n) = 3*a(n-1) + 4*a(n-2) - 5*a(n-3) - 14*a(n-4) - 17*a(n-5)
           + 18*a(n-6) + 29*a(n-7) + 23*a(n-8) + 12*a(n-9)
           - 27*a(n-10) - 27*a(n-11) - 16*a(n-12) - 7*a(n-13) - 5*a(n-14)
           + 18*a(n-15) - 9*a(n-16) + 9*a(n-17) - 3*a(n-18)   for n > 23
   Initial values:
   a(1)=4, a(2)=14, a(3)=45, a(4)=133, a(5)=419, a(6)=1382,
   a(7)=4495, a(8)=14754, a(9)=48525, a(10)=159584, a(11)=524981,
   a(12)=1727658, a(13)=5685321, a(14)=18710300, a(15)=61576790,
   a(16)=202655046, a(17)=666960573, a(18)=2195053580, a(19)=7224214965,
   a(20)=23775889227, a(21)=78249787781, a(22)=257531127865,
   a(23)=847571571085, a(24)=2789478970713 */
#include <stdlib.h>

/* Cached sequence values; a[0] holds a(1) */
static seq_t *cache = NULL;
/* Number of elements currently stored in cache */
static size_t cache_len = 0;

/* Initialise cache with the first 24 terms if it has not been done yet */
static void init_cache(void)
{
    if (cache != NULL) return;       /* already initialised */

    /* Allocate space for the first 24 terms */
    cache_len = 24;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    if (!cache) abort();             /* fatal if no memory */

    static const seq_t init[24] = {
        (seq_t)4,          (seq_t)14,          (seq_t)45,
        (seq_t)133,        (seq_t)419,         (seq_t)1382,
        (seq_t)4495,       (seq_t)14754,        (seq_t)48525,
        (seq_t)159584,     (seq_t)524981,      (seq_t)1727658,
        (seq_t)5685321,    (seq_t)18710300,    (seq_t)61576790,
        (seq_t)202655046,  (seq_t)666960573,   (seq_t)2195053580,
        (seq_t)7224214965, (seq_t)23775889227,(seq_t)78249787781,
        (seq_t)257531127865,(seq_t)847571571085,
        (seq_t)2789478970713
    };
    for (size_t i = 0; i < cache_len; ++i) cache[i] = init[i];
}

/* Ensure that cache holds values up to (but not including) index n+1
   (i.e. a(1)..a(n)).  If n exceeds current size, extend the array and
   compute the missing terms using the recurrence. */
static void ensure_cache(idx_t n)
{
    init_cache();                     /* make sure we have the 24 bases */

    if ((size_t)n <= cache_len) return;   /* already have the data */

    size_t old_len = cache_len;
    /* Grow at least to n, but never shrink */
    cache_len = (size_t)n;
    cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!cache) abort();

    /* Compute new entries; indices are 0‑based
       cache[i] holds a(i+1) for i >= 0.  We start at i = old_len. */
    for (size_t i = old_len; i < cache_len; ++i) {
        /* a(i+1) uses a(i), a(i-1), ... a(i-18) */
        seq_t val = 0;
        val += (seq_t)3 * cache[i-1];
        val += (seq_t)4 * cache[i-2];
        val -= (seq_t)5 * cache[i-3];
        val -= (seq_t)14 * cache[i-4];
        val -= (seq_t)17 * cache[i-5];
        val += (seq_t)18 * cache[i-6];
        val += (seq_t)29 * cache[i-7];
        val += (seq_t)23 * cache[i-8];
        val += (seq_t)12 * cache[i-9];
        val -= (seq_t)27 * cache[i-10];
        val -= (seq_t)27 * cache[i-11];
        val -= (seq_t)16 * cache[i-12];
        val -= (seq_t)7  * cache[i-13];
        val -= (seq_t)5  * cache[i-14];
        val += (seq_t)18 * cache[i-15];
        val -= (seq_t)9  * cache[i-16];
        val += (seq_t)9  * cache[i-17];
        val -= (seq_t)3  * cache[i-18];
        cache[i] = val;
    }
}

/* External linkage: the sequence function requested */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0, so return 0 */
    ensure_cache(n);
    return cache[n-1];                     /* cache[0] is a(1) */
}
