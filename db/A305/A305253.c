/*
 * A305253  Number of connected factorizations of n into factors greater than
 *          1 whose distinct factors are pairwise indivisible.
 * https://oeis.org/A305253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of connected factorizations of n into factors >1 *
 *            whose distinct factors are pairwise indivisible. The sequence is
 *            computed * by enumerating all unordered factorizations of n and
 *            keeping those that * satisfy the connectivity and indivisibility
 *            conditions
 * Range:     u8 n <= 20160, u16 n <= 20160, u32 n <= 20160, u64 n <= 20160,
 *            u128 n <= 20160, big any n
 * Validated: standard, pass, 121 terms through n = 9343 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Recursive DFS that enumerates all unordered factorizations */
static void _dfs(uint64_t rem, uint64_t start, uint64_t *curr,
                 int depth, seq_t *cnt)
{
    /* try all divisors f of rem with f >= start */
    for (uint64_t f = start; f <= rem / f; ++f)
    {
        if (rem % f == 0)
        {
            curr[depth] = f;
            _dfs(rem / f, f, curr, depth + 1, cnt);
        }
    }

    /* final factor: rem itself */
    if (rem >= start)
    {
        curr[depth] = rem;
        int m = depth + 1;                     /* number of factors */

        /* adjacency bitmask for connectivity */
        uint64_t adj[20] = {0};
        for (int i = 0; i < m; ++i)
        {
            for (int j = i + 1; j < m; ++j)
            {
                uint64_t a = curr[i], b = curr[j];
                /* compute gcd of a and b */
                uint64_t g = a, t = b;
                while (t)
                {
                    uint64_t r = g % t;
                    g = t;
                    t = r;
                }
                if (g > 1)
                {
                    adj[i] |= 1ULL << j;
                    adj[j] |= 1ULL << i;
                }
            }
        }

        /* BFS connectivity check */
        uint64_t visited = 1ULL;          /* vertex 0 visited */
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (int i = 0; i < m; ++i)
            {
                if (!(visited & (1ULL << i)) && (adj[i] & visited))
                {
                    visited |= 1ULL << i;
                    changed = true;
                }
            }
        }
        if (visited != ((1ULL << m) - 1))
            return;                       /* disconnected */

        /* pairwise indivisibility check */
        for (int i = 0; i < m; ++i)
        {
            for (int j = i + 1; j < m; ++j)
            {
                if (curr[i] != curr[j])
                {
                    if (curr[i] % curr[j] == 0 || curr[j] % curr[i] == 0)
                        return;           /* divisible pair found */
                }
            }
        }

        (*cnt)++;                         /* valid factorization */
    }
}

/* Sequence function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0ULL)          /* offset starts at 1; undefined for 0 */
        return (seq_t)0;

    /* static cache for n <= 10000 */
    static seq_t  cache[10001];
    static bool   computed[10001] = {false};

    if (n <= 10000ULL)
    {
        if (computed[n])
            return cache[n];
    }

    seq_t result = 0;
    uint64_t curr[20];      /* max depth needed for n <= 10000 is < 20 */

    _dfs((uint64_t)n, 2, curr, 0, &result);

    if (n <= 10000ULL)
    {
        cache[n] = result;
        computed[n] = true;
    }
    return result;
}
