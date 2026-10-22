/*
 * A349797  Number of non-weakly alternating permutations of the multiset of
 *          prime factors of n.
 * https://oeis.org/A349797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008480(n) - A349056(n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/factorial.h"

static uint64_t g_values[20];
static int      g_rem[20];
static int      g_U;
static int      g_totalL;
static bool     g_up_start;

/* Recursive helper: counts permutations satisfying the pattern
   starting at depth 'depth', with previous element index 'last_idx'. */
static seq_t dfs(int depth, int last_idx)
{
    if (depth == g_totalL)
        return (seq_t)1;

    bool need_le = g_up_start ? ((depth - 1) % 2 == 0) : ((depth - 1) % 2 == 1);
    seq_t ans = 0;

    for (int i = 0; i < g_U; ++i)
        if (g_rem[i] > 0)
        {
            if ((need_le && g_values[i] >= g_values[last_idx]) ||
                (!need_le && g_values[i] <= g_values[last_idx]))
            {
                --g_rem[i];
                ans += dfs(depth + 1, i);
                ++g_rem[i];
            }
        }
    return ans;
}

/* Counts weakly alternating permutations for a given starting
   comparison direction (up start = true for <= first, false for >=). */
static seq_t count_start(bool up_start)
{
    g_up_start = up_start;
    seq_t ans = 0;

    for (int i = 0; i < g_U; ++i)
        if (g_rem[i] > 0)
        {
            --g_rem[i];
            ans += dfs(1, i);
            ++g_rem[i];
        }

    return ans;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    uint64_t primes[20];
    uint64_t exps[20];
    size_t   cnt;

    factor_u64(n, primes, exps, &cnt);

    g_U = (int)cnt;
    g_totalL = 0;
    for (int i = 0; i < g_U; ++i)
    {
        g_values[i] = primes[i];
        g_rem[i]    = (int)exps[i];
        g_totalL   += g_rem[i];
    }

    if (g_totalL == 0)
        return 0;

    /* Total permutations of the multiset */
    seq_t total_perm = factorial((idx_t)g_totalL);
    for (int i = 0; i < g_U; ++i)
        if (g_rem[i] > 1)
            total_perm /= factorial((idx_t)g_rem[i]);

    /* Count weakly alternating permutations */
    seq_t up   = count_start(true);
    seq_t down = count_start(false);
    seq_t weakly = up + down;
    if (g_U == 1) /* all elements equal, counted twice */
        weakly -= 1;

    return total_perm - weakly;
}
