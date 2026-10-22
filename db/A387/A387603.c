/*
 * A387603  a(n) = (1/2) * Sum_{k=0..floor(n/3)} 2^(n-k) *
 *          binomial(2*n-4*k+2,2*k+1).
 * https://oeis.org/A387603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 4*a(n-1) - 4*a(n-2) + 8*a(n-3) + 16*a(n-4) - 16*a(n-6),
   a(0)=1, a(1)=4, a(2)=12, a(3)=40, a(4)=160, a(5)=640 */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored (highest index+1) */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Ensure that cache can hold at least 'needed' elements */
static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise base values if they are not yet stored */
static void init_base(void)
{
    if (cache_size >= 6)
        return;
    ensure_cap(6);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)12;
    cache[3] = (seq_t)40;
    cache[4] = (seq_t)160;
    cache[5] = (seq_t)640;
    cache_size = 6;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    init_base();

    size_t i = (size_t)n;
    if (i < cache_size)
        return cache[i];

    ensure_cap(i + 1);

    for (size_t j = cache_size; j <= i; ++j)
    {
        /* For j >= 6 the recurrence is valid */
        seq_t val;
        val = (seq_t)4  * cache[j-1]
            - (seq_t)4  * cache[j-2]
            + (seq_t)8  * cache[j-3]
            + (seq_t)16 * cache[j-4]
            - (seq_t)16 * cache[j-6];
        cache[j] = val;
    }

    cache_size = i + 1;
    return cache[i];
}
