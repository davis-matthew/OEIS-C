/*
 * A242317  Number of length n+2+2 0..2 arrays with every value 0..2 appearing
 *          at least once in every consecutive 2+3 elements, and new values
 *          0..2 introduced in order.
 * https://oeis.org/A242317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) + 2*a(n‑2) + 2*a(n‑3) + 2*a(n‑4) - a(n‑5) - a(n‑6)
 *            Initial values: a(1) = 25, a(2) = 61, a(3) = 145, a(4) = 337,
 *            a(5) = 781, a(6) = 1829
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 49, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Cache up to the largest n seen; size 200000 is safe for n ≤ 10000 */
    static seq_t cache[200000];
    static idx_t cache_len = 0;      /* number of terms currently cached */

    /* Initialise the first six terms on first entry */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)25;    /* a(1) */
        cache[1] = (seq_t)61;    /* a(2) */
        cache[2] = (seq_t)145;   /* a(3) */
        cache[3] = (seq_t)337;   /* a(4) */
        cache[4] = (seq_t)781;   /* a(5) */
        cache[5] = (seq_t)1829;  /* a(6) */
        cache_len = 6;
    }

    /* No need to recompute if n is already cached */
    if (n <= cache_len)
        return cache[(size_t)n - 1];

    /* Ensure we do not exceed the static buffer */
    assert(n <= 200000U);

    /* Compute missing terms on demand */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* array indices are zero–based */
        seq_t a1 = cache[(size_t)i - 2];  /* a(i‑1)   */
        seq_t a2 = cache[(size_t)i - 3];  /* a(i‑2)   */
        seq_t a3 = cache[(size_t)i - 4];  /* a(i‑3)   */
        seq_t a4 = cache[(size_t)i - 5];  /* a(i‑4)   */
        seq_t a5 = cache[(size_t)i - 6];  /* a(i‑5)   */
        seq_t a6 = cache[(size_t)i - 7];  /* a(i‑6)   */

        cache[(size_t)i - 1] = a1 + (seq_t)2 * a2 + (seq_t)2 * a3
                               + (seq_t)2 * a4 - a5 - a6;
    }

    cache_len = n;
    return cache[(size_t)n - 1];
}
