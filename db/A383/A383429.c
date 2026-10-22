/*
 * A383429  a(1)=1, and for n>1, a(n) = a(n-1) concatenated with the length of
 *          the decimal representation of a(n-1).
 * https://oeis.org/A383429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = concat(a(n-1), digit_len(a(n-1)))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   digit_len, digit_len_impl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  /*  Where concat(x,y) = x·10^{digit_len(y)} + y and digit_len(z)
    is the number of decimal digits of z.  */

/*  The function a(idx_t n) is memoised so that any order or number
    of calls is handled efficiently.  */

#include <stdlib.h>

/*  Use the supplied integer types.  */
extern seq_t a(idx_t n);          /* forward declaration for the build system */

/*  For arbitrary‑precision builds the helper digits functions are
    provided by the library.  For primitive integer types we implement
    the required helpers ourselves.  */

#if defined(__SIZEOF_INT128__)  /* arbitrary precision build */
#include "../utils/digit_len.h"
#include "../utils/digit_len_impl.h"
#else

#define digit_len(x) digit_len_impl((x))
#endif

/*  Compute 10^e as a seq_t value.  This works for all seq_t.  */
static seq_t pow10_exp(unsigned int e)
{
    seq_t r = 1;
    for (unsigned int i = 0; i < e; ++i)
        r = r * (seq_t)10;
    return r;
}

/*  Memoisation cache.  */ 
static seq_t *cache      = NULL;   /* sequence values                      */
static idx_t  cache_max  = 0;      /* largest index stored (>= cache_max)  */
static idx_t  cache_size = 0;      /* allocated size of cache (indices 0..size-1) */

/*  Main sequence routine.  */
seq_t a(idx_t n)
{
    if (n < 1)                      /* undefined for n < 1 */
        return (seq_t)0;

    /*  Initialise cache lazily.  */
    if (cache == NULL) {
        cache_size = 2;             /* indices 0 and 1 are needed */
        cache      = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (!cache)
            return (seq_t)0;        /* allocation failure */
        cache[0] = 0;               /* unused placeholder */
        cache[1] = (seq_t)1;        /* a(1) */
        cache_max = 1;
    }

    /*  Return cached value if available.  */
    if (n <= cache_max)
        return cache[n];

    /*  Grow cache to accommodate index n if necessary.  */
    if (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < n + 1)
            new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp)
            return (seq_t)0;        /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /*  Build missing terms from cache_max + 1 up to n.  */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        int len       = digit_len(prev);                   /* digits of prev   */
        int len_len   = digit_len((seq_t)len);             /* digits of len    */
        seq_t factor  = pow10_exp((unsigned int)len_len);  /* 10^{len_len}     */

        cache[i] = prev * factor + (seq_t)len;
    }
    cache_max = n;

    return cache[n];
}
