/*
 * A368483  The degree of polynomials related to Somos-7 sequences. Also for n
 *          > 6 the degree of the (n-6)-th involution in a family of
 *          involutions in the Cremona group of rank 6 defined by a Somos-7
 *          sequence.
 * https://oeis.org/A368483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-7) - a(n-8) + a(n-10) initial
 *            values: a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0, a(7)=2, a(8)=3,
 *            a(9)=4, a(10)=5 (the recurrence is valid for all n ≥ 10)
 * Range:     u8 n <= 49, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t  cached = 0;       /* number of terms stored in cache */
static size_t  capacity = 0;     /* allocated capacity */

/* ensure cache has at least new_cap slots */
static void ensure_cache_size(size_t new_cap)
{
    if (capacity >= new_cap)
        return;
    size_t new_cap2 = capacity ? capacity * 2 : 16;
    if (new_cap2 < new_cap)
        new_cap2 = new_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap2 * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    capacity = new_cap2;
}

/* initialise cache with known initial values up to index 9 */
static void init_cache(void)
{
    if (cached)
        return;
    ensure_cache_size(10); /* indices 0 … 9 */

    /* indices 0‑6 are 0 */
    for (idx_t i = 0; i <= 6; ++i)
        cache[i] = 0;

    /* known values */
    cache[7] = 2;
    cache[8] = 3;
    cache[9] = 4;

    cached = 10; /* 0 … 9 are stored */
}

seq_t a(idx_t n)
{
    init_cache();

    size_t idx = (size_t)n;
    if (idx >= capacity)
        ensure_cache_size(idx + 1);

    if (idx >= cached) {
        /* compute a(n) for n = cached … idx using recurrence */
        for (size_t i = cached; i <= idx; ++i) {
            /* recurrence a(i) = a(i-2)+a(i-3)-a(i-7)-a(i-8)+a(i-10) */
            seq_t sum = cache[i - 2] + cache[i - 3];
            sum += cache[i - 10];
            sum -= cache[i - 7] + cache[i - 8];
            cache[i] = sum;
        }
        cached = idx + 1;
    }

    return cache[idx];
}
