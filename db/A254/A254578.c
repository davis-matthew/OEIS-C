/*
 * A254578  Number of ordered factorizations into distinct factors.
 * https://oeis.org/A254578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{S ⊂ {f > 1 | ∏f = n}} |S|! (OEIS A254578)
 * Range:     u8 n <= 479, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* pre‑compute factorials up to 11!; 12 is enough for n ≤ 10000 */
static seq_t fact[12];
static bool fact_inited = false;

static void init_fact(void)
{
    if (!fact_inited) {
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i < 12; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_inited = true;
    }
}

/* depth‑first search that enumerates each unordered set of distinct
   factors ≥2 that multiply to rem.  Each set of size k contributes k! */
static void dfs(idx_t rem, idx_t start, idx_t depth, seq_t *sum)
{
    if (rem == (idx_t)1) {
        *sum += fact[depth];
        return;
    }

    /* choose rem itself as a factor (completing the set) */
    if (rem >= start)
        *sum += fact[depth + 1];

    for (idx_t f = start; f <= rem / f; ++f) {
        if (rem % f == 0)
            dfs(rem / f, f + 1, depth + 1, sum);
    }
}

seq_t a(idx_t n)
{
    init_fact();
    seq_t total = (seq_t)0;
    dfs(n, (idx_t)2, (idx_t)0, &total);
    return total;
}
