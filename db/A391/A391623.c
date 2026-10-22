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
 * Range:     u8 n <= 153, u16 n <= 9666, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_multiple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/has_multiple.h"

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
