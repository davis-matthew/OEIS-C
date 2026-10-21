/*
 * A291225  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S)^5.
 * https://oeis.org/A291225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 29, u64 n <= 69, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A291225 (OEIS).  The values satisfy the linear recurrence
 *
 *   a(n) = 5*a(n-1) - 5*a(n-2) - 10*a(n-3) + 15*a(n-4) + 11*a(n-5)
 *          - 15*a(n-6) - 10*a(n-7) + 5*a(n-8) + 5*a(n-9) + a(n-10)
 *
 *   with initial terms
 *   a(0)=5, a(1)=15, a(2)=40, a(3)=100, a(4)=236,
 *   a(5)=535, a(6)=1175, a(7)=2515, a(8)=5270, a(9)=10846, a(10)=21980.
 *
 * The function returns the n‑th term for any n ≥ 0.  Results are computed
 * modulo the width of seq_t (wrap around for 8‑bit, 16‑bit, etc.), which
 * satisfies the requirement that values fit in seq_t are exact.
 *
 * Note: seq_t and idx_t are typedefs supplied by the build system.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array of computed values            */
static size_t cache_size = 0;    /* number of values currently stored   */
static size_t cache_cap  = 0;    /* allocated capacity of the array      */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p)
        abort();                    /* out of memory */
    cache      = p;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 11 known terms. */
static void init_cache(void)
{
    if (cache == NULL) {
        ensure_cache_cap(11);
        cache[0]  = (seq_t)5;
        cache[1]  = (seq_t)15;
        cache[2]  = (seq_t)40;
        cache[3]  = (seq_t)100;
        cache[4]  = (seq_t)236;
        cache[5]  = (seq_t)535;
        cache[6]  = (seq_t)1175;
        cache[7]  = (seq_t)2515;
        cache[8]  = (seq_t)5270;
        cache[9]  = (seq_t)10846;
        cache[10] = (seq_t)21980;
        cache_size = 11;
    }
}

/* Public function producing a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    init_cache();

    size_t idx = (size_t)n;
    if (idx < cache_size)
        return cache[idx];

    ensure_cache_cap(idx + 1);
    for (size_t i = cache_size; i <= idx; ++i) {
        /* Compute a(i) using the recurrence. */
        seq_t val =
            (seq_t)( 5  * cache[i-1]
                  - 5  * cache[i-2]
                  - 10 * cache[i-3]
                  + 15 * cache[i-4]
                  + 11 * cache[i-5]
                  - 15 * cache[i-6]
                  - 10 * cache[i-7]
                  + 5  * cache[i-8]
                  + 5  * cache[i-9]
                  +    cache[i-10] );
        cache[i] = val;
    }
    cache_size = idx + 1;
    return cache[idx];
}
