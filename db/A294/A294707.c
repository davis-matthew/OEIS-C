/*
 * A294707  Number of permutations of [n] avoiding {4321, 3142, 1234}.
 * https://oeis.org/A294707
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

/* 
   Computation of OEIS A294707:
   number of permutations of [n] avoiding the patterns {4321, 3142, 1234}.
   For n ≥ 10 the result is 0 by Erdos–Szekeres.
   For n ≤ 9 we enumerate all permutations and count those that avoid
   all three patterns. Results are memoised for subsequent calls.
*/

#include <stdbool.h>

/* check whether a permutation of length n avoids the three patterns   */
static bool avoid_pattern(const int *p, int n)
{
    for (int i = 0; i <= n - 4; ++i)
        for (int j = i + 1; j <= n - 3; ++j)
            for (int k = j + 1; k <= n - 2; ++k)
                for (int l = k + 1; l <= n - 1; ++l) {
                    int a = p[i], b = p[j], c = p[k], d = p[l];
                    /* pattern 1234 */
                    if (a < b && b < c && c < d) return false;
                    /* pattern 4321 */
                    if (a > b && b > c && c > d) return false;
                    /* pattern 3142 :  small < second small < second big < big */
                    if (b < d && d < a && a < c) return false;
                }
    return true;
}

/* ones that avoid the forbidden patterns.                            */
static void generate(int n, int pos, int *perm, bool *used, seq_t *cnt)
{
    if (pos == n) {
        if (avoid_pattern(perm, n))
            (*cnt)++;
    } else {
        for (int v = 1; v <= n; ++v) {
            if (!used[v]) {
                used[v] = true;
                perm[pos] = v;
                generate(n, pos + 1, perm, used, cnt);
                used[v] = false;
            }
        }
    }
}

/* maximum n for non‑zero values (+1 for zero index) */
#define MAX_PRECAL 10

/* memoised results (a(0) = 1, a(1) = 1, …, a(9)) */
static seq_t precomp[MAX_PRECAL] = {1};
static bool ready[MAX_PRECAL] = {true};

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* recursive generation of all permutations of 1..n, counting the     */

/* ------------------------------------------------------------------ */
/* public function requested by the build system                       */
seq_t a(idx_t n)
{
    if (n >= (idx_t)MAX_PRECAL)            /* n ≥ 10 → answer is zero   */
        return 0;

    int ni = (int)n;                       /* safe to cast, max 9       */
    if (!ready[ni]) {
        int perm[MAX_PRECAL];
        bool used[MAX_PRECAL];
        for (int i = 1; i <= ni; ++i) used[i] = false;
        seq_t cnt = 0;
        generate(ni, 0, perm, used, &cnt);
        precomp[ni] = cnt;
        ready[ni] = true;
    }
    return precomp[ni];
}
