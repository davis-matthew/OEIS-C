/*
 * A301454  Number of strictly log-concave permutations of {1,...,n}.
 * https://oeis.org/A301454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A301454: number of strictly log‑concave permutations of {1,…,n} */
#include <stdbool.h>

/* Recursive helper that generates permutations and counts those that
   satisfy the strict log‑concavity condition. */
static void permute(idx_t depth, idx_t n, seq_t *perm, bool *used, seq_t *cnt)
{
    if (depth == n) {
        /* Verify strict log‑concavity: p[i]^2 > p[i-1] * p[i+1] for all 1 ≤ i ≤ n-2. */
        for (idx_t i = 1; i + 1 < n; ++i) {
            seq_t left  = perm[i-1] * perm[i+1];
            seq_t right = perm[i] * perm[i];
            if (right <= left) return;          /* fails the inequality */
        }
        (*cnt)++;                               /* valid permutation counted */
        return;
    }
    for (idx_t i = 0; i < n; ++i) {
        if (!used[i]) {
            used[i] = true;
            perm[depth] = (seq_t)(i + 1);       /* elements are 1..n */
            permute(depth + 1, n, perm, used, cnt);
            used[i] = false;
        }
    }
}

/* Count strictly log‑concave permutations of {1..n} for n ≤ 8. */
static seq_t count_logconcave(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t perm[9];
    bool used[9] = {false};
    seq_t cnt = 0;
    permute(0, n, perm, used, &cnt);
    return cnt;
}

/* Public entry function. */
seq_t a(idx_t n)
{
    /* Cache the computed values for n < 9. */
    static seq_t cache[9];
    static bool ready = false;
    if (!ready) {
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = count_logconcave(i);
        ready = true;
    }

    if (n < 9)
        return cache[n];

    /* For n ≥ 9 the sequence stabilises at 8. */
    return (seq_t)8;
}
