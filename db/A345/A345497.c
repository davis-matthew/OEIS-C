/*
 * A345497  Numbers that are the sum of eight squares in ten or more ways.
 * https://oeis.org/A345497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 74 (for n > 10) * Recurrence used for n < 11: numbers
 *            are found by brute‑force * enumeration of unordered 8‑square
 *            representations
 * Range:     u8 n <= 181, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static void dfs_rec(idx_t depth, idx_t last, int sum, idx_t max_root,
                    unsigned int *cnt)
{
    if (depth == 8) {
        if (sum <= 200U)
            ++cnt[sum];
        return;
    }
    for (idx_t v = last; v <= max_root; ++v) {
        int ns = sum + v * v;
        if (ns > 200U)
            break;
        dfs_rec(depth + 1, v, ns, max_root, cnt);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t first[10];
    static unsigned int cnt[201];   /* zero‑initialized */

    if (!ready) {
        /* find maximum root needed for sums up to 200 */
        idx_t max_root = 0;
        while ((max_root + 1U) * (max_root + 1U) <= 200U)
            ++max_root;

        /* enumerate all non‑decreasing 8‑tuples of positive integers */
        dfs_rec(0, 1, 0, max_root, cnt);

        /* collect the first 10 integers with at least 10 representations */
        idx_t k = 0;
        for (int s = 1; s <= 200 && k < 10; ++s) {
            if (cnt[s] >= 10)
                first[k++] = (seq_t)s;
        }
        ready = true;
    }

    if (n <= 10)
        return first[(idx_t)(n - 1U)];

    /* for n > 10 the closed‑form holds */
    return (seq_t)n + (seq_t)74;
}
