/*
 * A294706  Number of permutations of [n] avoiding {4321, 3412, 1234}.
 * https://oeis.org/A294706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 10 terms through n = 9 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of permutations of [n] avoiding patterns {4321, 3412, 1234}. */
/* For n >= 10 the value is 0 because any sequence of length 10 contains either
   1234 or 4321 (Erdős–Szekeres theorem).  The remaining values for 0 <= n <= 9
   are obtained by brute‑force enumeration. */

#include <stdbool.h>
#include <stddef.h>

/* Helper: test whether a permutation of length n avoids all forbidden patterns */
static bool avoid_patterns(int n, const int *perm)
{
    if (n < 4)
        return true;  /* No 4‑letter pattern can appear */

    for (int i = 0; i < n - 3; ++i) {
        int a = perm[i];
        for (int j = i + 1; j < n - 2; ++j) {
            int b = perm[j];
            for (int k = j + 1; k < n - 1; ++k) {
                int c = perm[k];
                for (int l = k + 1; l < n; ++l) {
                    int d = perm[l];

                    /* 1234 pattern: strictly increasing */
                    if (a < b && b < c && c < d)
                        return false;

                    /* 4321 pattern: strictly decreasing */
                    if (a > b && b > c && c > d)
                        return false;

                    /* 3412 pattern: ranks 3,4,1,2 in relative order */
                    /* Equivalent conditions:  b is largest, a second largest,
                       d second smallest, c smallest. */
                    if (b > a && b > c && b > d &&
                        a > c && a > d &&
                        d > c)
                        return false;
                }
            }
        }
    }
    return true;
}

#define MAX_N 9  /* Maximum n for which non‑zero terms exist */

/* Recursive generation of all permutations of size n */
static void generate(int n, int idx, int *perm, bool *used, seq_t *cnt)
{
    if (idx == n) {
        if (avoid_patterns(n, perm))
            (*cnt)++;
        return;
    }
    for (int v = 1; v <= n; ++v) {
        if (!used[v]) {
            used[v] = true;
            perm[idx] = v;
            generate(n, idx + 1, perm, used, cnt);
            used[v] = false;
        }
    }
}

/* Compute the cache of values for n = 0..9 */
static void init_cache(seq_t *cache)
{
    static bool inited = false;
    if (inited)
        return;
    inited = true;

    /* n = 0 */
    cache[0] = (seq_t)1;

    int perm[MAX_N];
    bool used[MAX_N + 1];

    for (int m = 1; m <= MAX_N; ++m) {
        seq_t cnt = 0;
        for (int i = 1; i <= m; ++i)
            used[i] = false;
        generate(m, 0, perm, used, &cnt);
        cache[m] = cnt;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t cache[MAX_N + 1];
    init_cache(cache);

    if (n < (idx_t)(MAX_N + 1))
        return cache[(size_t)n];
    else
        return (seq_t)0;
}
