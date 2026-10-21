/*
 * A271217  Number of symmetric reduced rearrangement maps.
 * https://oeis.org/A271217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=2 a(2k+1)=2*a(2k)+a(2k-1) a(2k)
 *            =(2k-1)*a(2k-1)+(2k-2)*a(2k-3) (k≥1) (OEIS A271217)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* for malloc / realloc */
#include <stdint.h>      /* for uint64_t, etc. (types are supplied by the build) */

/* Static storage for memoisation */
static seq_t *cache = NULL;      /* array of computed terms            */
static idx_t  cap   = 0;         /* current capacity (number of elements)  */
static idx_t  max_n = (idx_t)-1; /* largest index computed so far        */

/* Ensure that the cache can hold indices 0 … need-1 */
static void ensure_capacity(idx_t need)
{
    if (need <= cap)
        return;

    /* grow capacity by powers of two, start at 16 if zero */
    idx_t new_cap = (cap) ? cap : 16;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache,
                                        (size_t)new_cap * sizeof(seq_t));
    cache = new_cache;
    cap   = new_cap;
}

/* Compute the n-th term of A271217 */
seq_t a(idx_t n)
{
    /* Initialise base values on first call */
    if (cache == NULL) {
        ensure_capacity(3);          /* need indices 0,1,2 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        max_n    = 2;
    }

    if (n <= max_n)
        return cache[n];

    /* Make sure the cache is large enough for n */
    ensure_capacity(n + 1);

    /* Compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if ((i & 1U) == 0) {          /* even i = 2k  */
            idx_t k = i >> 1;
            seq_t term1 = cache[i - 1] * (seq_t)(2 * k - 1);
            seq_t term2 = cache[i - 3] * (seq_t)(2 * k - 2);
            cache[i] = term1 + term2;
        } else {                     /* odd i = 2k+1 */
            cache[i] = (cache[i - 1] * (seq_t)2) + cache[i - 2];
        }
    }

    max_n = n;
    return cache[n];
}
