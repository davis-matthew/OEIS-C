/*
 * A289693  The number of partitions of [n] with exactly 3 blocks without
 *          peaks.
 * https://oeis.org/A289693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 24*a(n-3) - 29*a(n-4) + 25*a(n-5)
 *            - 17*a(n-6) + 9*a(n-7) - 3*a(n-8) + a(n-9) Initial values:
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=3, a(5)=9, a(6)=27, a(7)=75,
 *            a(8)=197, a(9)=503
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;

    static seq_t *cache = NULL;   /* indices start at 1 */
    static size_t cap   = 0;       /* allocated size of cache   */
    static size_t last  = 0;       /* highest index computed     */

    const size_t min_cap = 10;     /* we need indices 1..9 at least */

    /* Ensure cache big enough for indices up to n. */
    size_t required = (size_t)n + 1;
    if (required < min_cap)
        required = min_cap;
    if (required > cap)
    {
        seq_t *tmp = alloc_seq(required);
        if (!tmp)
            return (seq_t)0; /* allocation failure */

        /* copy old values */
        for (size_t i = 0; i < cap; ++i)
            tmp[i] = cache[i];

        free_seq(cache);
        cache = tmp;
        cap   = required;
    }

    /* Initialize first nine terms only once. */
    if (last == 0)
    {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)9;
        cache[6] = (seq_t)27;
        cache[7] = (seq_t)75;
        cache[8] = (seq_t)197;
        cache[9] = (seq_t)503;
        last = 9;
    }

    if ((size_t)n <= last)
        return cache[(size_t)n];

    /* Compute terms up to n using the 9‑term linear recurrence. */
    for (size_t i = last + 1; i <= (size_t)n; ++i)
    {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        seq_t a7 = cache[i - 7];
        seq_t a8 = cache[i - 8];
        seq_t a9 = cache[i - 9];

        seq_t pos = a1 * 6 + a3 * 24 + a5 * 25 + a7 * 9 + a9;
        seq_t neg = a2 * 15 + a4 * 29 + a6 * 17 + a8 * 3;

        cache[i] = pos - neg;
    }

    last = (size_t)n;
    return cache[(size_t)n];
}
