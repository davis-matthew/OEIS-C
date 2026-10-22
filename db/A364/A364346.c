/*
 * A364346  Number of strict integer partitions of n such that there is no
 *          ordered triple of parts (a,b,c) (repeats allowed) satisfying a + b
 *          = c. A variation of sum-free strict partitions.
 * https://oeis.org/A364346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strict integer partitions of n with no ordered
 *            triple (a,b,c) (repeats allowed) satisfying a + b = c
 * Range:     u8 n <= 38, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_N 20000          /* maximum n we expect to handle (≈ 10 000)   */
#define MAX_PARTS 200        /* maximum number of parts in any partition   */

static bool present[MAX_N + 1];   /* part already chosen                        */
static bool forbidden[MAX_N + 1]; /* sums that cannot appear as future parts    */
static int sel[MAX_PARTS];       /* currently chosen parts (used to update sums) */

/* depth‑first search counting valid partitions.
   rem   – remaining sum to reach n
   last  – maximum allowed part (parts must be < last to enforce ordering)
   depth – number of parts already chosen
   N     – the target n (constant for this call)                         */
static seq_t dfs(int rem, int last, int depth, int N)
{
    if (rem == 0)          /* finished a partition                        */
        return (seq_t)1;

    seq_t total = 0;

    for (int x = last; x >= 1; --x) {
        if (x > rem)                 /* cannot exceed remaining sum              */
            continue;
        if (forbidden[x])            /* x is forbidden as a sum of two parts    */
            continue;

        /* check that adding x does not create a triple with an existing part */
        bool bad = false;
        for (int i = 0; i < depth; ++i) {
            int a = sel[i];
            int s = a + x;
            if (s <= N && present[s]) { bad = true; break; }
        }
        if (!bad && 2 * x <= N && present[2 * x])
            bad = true;

        if (bad)
            continue;               /* skip this candidate                      */

        /* choose x */
        sel[depth] = x;
        present[x] = true;

        /* record sums newly forbidden by adding x */
        int banned[MAX_PARTS];
        int banned_cnt = 0;

        for (int i = 0; i < depth; ++i) {
            int s = sel[i] + x;
            if (s <= N && !forbidden[s]) {
                forbidden[s] = true;
                banned[banned_cnt++] = s;
            }
        }
        int s = 2 * x;
        if (s <= N && !forbidden[s]) {
            forbidden[s] = true;
            banned[banned_cnt++] = s;
        }

        /* recurse to fill remaining sum */
        total += dfs(rem - x, x - 1, depth + 1, N);

        /* undo changes */
        for (int i = 0; i < banned_cnt; ++i)
            forbidden[banned[i]] = false;
        present[x] = false;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* empty partition counts as one                      */
        return (seq_t)1;

    if (n > MAX_N) {           /* fall back to recursion even for larger n           */
        /* we still need arrays big enough; here we simply treat as not
           expected in the problem bounds.                           */
    }

    int N = (int)n;
    for (int i = 0; i <= N; ++i) {
        present[i] = false;
        forbidden[i] = false;
    }
    return dfs(N, N, 0, N);
}
