/*
 * A338160  Number of ways to represent n as a product of the greatest number
 *          of distinct factors.
 * https://oeis.org/A338160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A338160:
   a(n) = number of ways to write n as a product of distinct factors > 1
          using the greatest possible number of factors.
   a(1) = 1, and for n > 1 the calculation is performed recursively
   by exploring all strictly increasing factorizations. */

#include <stdint.h>
#include <stdbool.h>

/* ---------- integer square‑root for uint64_t (no <math.h> or floating‑point) */
static inline uint64_t u64_sqrt(uint64_t n)
{
    uint64_t lo = 0, hi = n >> 1u | 1u;   /* hi >= sqrt(n) for n>1 */
    while (lo < hi)
    {
        uint64_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid > 0 && mid > n / mid)     /* mid*mid > n, adjust hi */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* ---------- recursive search for all distinct‑factor factorizations */
static void dfs(uint64_t remaining, uint64_t min_factor, int depth,
                int *max_depth, seq_t *cnt)
{
    /* Use the remaining product itself as the last factor. */
    if (remaining > 1 && remaining >= min_factor)
    {
        int final_depth = depth + 1;
        if (final_depth > *max_depth)
        {
            *max_depth = final_depth;
            *cnt = (seq_t)1;
        }
        else if (final_depth == *max_depth)
        {
            *cnt += (seq_t)1;
        }
    }

    /* Recurse on proper divisors f of remaining, with f >= min_factor. */
    uint64_t limit = u64_sqrt(remaining);
    for (uint64_t f = min_factor; f <= limit; ++f)
    {
        if (remaining % f == 0)
        {
            uint64_t quo = remaining / f;
            if (quo > f)  /* keeps all factors distinct and strictly increasing */
                dfs(quo, f + 1, depth + 1, max_depth, cnt);
        }
    }
}

/* ---------- main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;       /* a(1) = 1 */

    uint64_t nn = (uint64_t)n;
    int max_depth = 0;
    seq_t cnt = 0;
    dfs(nn, 2, 0, &max_depth, &cnt);
    return cnt;
}
