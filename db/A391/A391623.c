/*
 * A391623  Positive integers k such that there is more than one nonnegative
 *          sequence whose first sums are the k-th composition in standard
 *          order.
 * https://oeis.org/A391623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is the n-th positive integer k for which there is more than
 *            one nonnegative sequence whose first sums equal the k-th
 *            composition in graded reverse‑lexicographic (standard) order.
 *            The value of a(n) is computed by enumerating all compositions in
 *            that order and counting those that admit at leas...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Number of integer values t that satisfy the constraints:
   a_i = (-1)^{i-1} t + s_i ,   0 <= a_i .
   This returns true if there are at least two such values. */
static bool has_multiple(const int *c, int len)
{
    int64_t s[65];
    s[0] = 0;
    for (int i = 0; i < len; ++i) {
        s[i + 1] = (int64_t)c[i] - s[i];
    }

    int64_t L = 0;                  /* lower bound for t               */
    int64_t U = INT64_MAX;          /* upper bound for t               */
    for (int i = 0; i <= len; ++i) {
        if ((i & 1) == 0) {         /* even index → coefficient +1   */
            int64_t val = -s[i];
            if (val > L) L = val;
        } else {                    /* odd index  → coefficient -1   */
            int64_t val = s[i];
            if (val < U) U = val;
        }
    }
    if (U < L) return false;
    return (U - L + 1) > 1;
}

/* ---------- helpers ---------- */

static int comp[64];            /* buffer for a composition          */
static idx_t g_k;               /* current composition index (k)    */
static idx_t g_found;           /* how many of them are in the set  */
static idx_t g_target;          /* requested n in a(n)               */
static seq_t g_answer;          /* currently found answer           */
static bool g_done;             /* finished flag                     */

/* Recursively generate all RL compositions of 'rem' and process them. */
static void gen_rl(int rem, int depth)
{
    if (g_done) return;

    if (rem == 0) {                     /* a complete composition   */
        ++g_k;
        if (has_multiple(comp, depth)) {
            ++g_found;
            if (g_found == g_target) {
                g_answer = (seq_t)g_k;
                g_done = true;
            }
        }
        return;
    }

    for (int last = rem; last >= 1 && !g_done; --last) {
        comp[depth] = last;
        gen_rl(rem - last, depth + 1);
    }
}

/* ---------- sequence a(n) ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safeguard: offset starts at 1  */
    g_k = 0;
    g_found = 0;
    g_target = n;
    g_done = false;
    g_answer = 0;

    int total = 1;
    while (!g_done) {
        gen_rl(total, 0);
        ++total;
    }
    return g_answer;
}
