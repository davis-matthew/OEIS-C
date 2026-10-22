/*
 * A350414  a(1)=1; for n > 1, a(n) = a(n-1) + (sum of odd-indexed digits of
 *          a(n-1)) - (sum of even-indexed digits of a(n-1)).
 * https://oeis.org/A350414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) + (sum of odd-indexed digits
 *            of a(n-1)) – (sum of even-indexed digits of a(n-1)) The code
 *            memoises past values so that calls are efficient regardless of
 *            the order of n
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void sum_odd_even(seq_t value, seq_t *odd, seq_t *even)
{
    seq_t o = 0;
    seq_t e = 0;
    idx_t pos = 1;
    while (value)
    {
        seq_t d = value % (seq_t)10;
        if (pos & 1)
            o += d;
        else
            e += d;
        value /= (seq_t)10;
        pos++;
    }
    *odd = o;
    *even = e;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0, but harmless */

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t count = 0;

    /* First call: initialise cache */
    if (!cache)
    {
        capacity = 128;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t)); /* cast for C++ */
        cache[0] = (seq_t)1;  /* a(1) */
        count = 1;
    }

    /* Grow cache if needed */
    if ((size_t)n > capacity)
    {
        size_t newcap = capacity ? capacity * 2 : 64;
        if (newcap < (size_t)n)
            newcap = (size_t)n;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t)); /* cast for C++ */
        capacity = newcap;
    }

    /* Compute terms up to n */
    for (size_t i = count; i < (size_t)n; ++i)
    {
        seq_t prev = cache[i - 1];
        seq_t odd, even;
        sum_odd_even(prev, &odd, &even);
        cache[i] = prev + odd - even;
    }
    count = (size_t)n;

    return cache[(size_t)n - 1];
}
