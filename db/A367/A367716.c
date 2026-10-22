/*
 * A367716  G.f. A(x) satisfies A(x) = 1 / ((1 + x) * (1 - x * (1 + x) *
 *          A(x^2))).
 * https://oeis.org/A367716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=0..n-1} a(floor(k/2)) * a(n-1-k)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* sequence cache */
static size_t cap   = 0;      /* allocated capacity */
static size_t len   = 0;      /* number of values actually stored */

/* enlarge cache so it can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;
    size_t newcap = cap ? cap * 2 : 2;
    while (newcap < needed)
        newcap *= 2;
    seq_t *newbuf = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newbuf)
        exit(1);         /* OOM: caller never expects this */
    if (cache)
    {
        for (size_t i = 0; i < len; ++i)
            newbuf[i] = cache[i];
        free(cache);
    }
    cache = newbuf;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    /* initialise cache[0] lazily */
    if (len == 0)
    {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        len = 1;
    }

    if (n < len)
        return cache[n];

    ensure_capacity((size_t)n + 1);

    /* compute all missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i)
    {
        idx_t m = (idx_t)(i - 1);          /* n-1 in the recurrence */
        seq_t sum = 0;

        /* iterate over j = floor(k/2) */
        for (idx_t j = 0, twoj = 0; twoj <= m; ++j, twoj += 2)
        {
            seq_t term = cache[j];
            seq_t v1   = cache[(idx_t)(m - twoj)];
            sum += term * v1;
            if (twoj + 1 <= m)
            {
                seq_t v2 = cache[(idx_t)(m - twoj - 1)];
                sum += term * v2;
            }
        }

        /* apply the ±1 shift depending on parity of i */
        if ((i & 1ULL) == 0)      /* even i -> +1 */
            cache[i] = sum + (seq_t)1;
        else                      /* odd i  -> -1 */
            cache[i] = sum - (seq_t)1;
    }

    return cache[n];
}
