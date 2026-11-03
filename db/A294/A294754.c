/*
 * A294754  Number of permutations of [n] avoiding {4321, 4123, 1234}.
 * https://oeis.org/A294754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 10 terms through n = 9 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool patterns_present(const int *perm, int n) {
    for (int i = 0; i < n - 3; ++i) {
        for (int j = i + 1; j < n - 2; ++j) {
            for (int k = j + 1; k < n - 1; ++k) {
                for (int l = k + 1; l < n; ++l) {
                    int a = perm[i], b = perm[j], c = perm[k], d = perm[l];
                    /* 4321 */
                    if (a > b && b > c && c > d) return true;
                    /* 1234 */
                    if (a < b && b < c && c < d) return true;
                    /* 4123 : a is largest, b<c<d are increasing */
                    if (a > b && b < c && c < d && a > d) return true;
                }
            }
        }
    }
    return false;
}

static void gen_perms_rec(int depth, int n, int *perm, char *used, seq_t *cnt) {
    if (depth == n) {
        if (!patterns_present(perm, n)) (*cnt)++;
        return;
    }
    for (int v = 0; v < n; ++v) {
        if (!used[v]) {
            used[v] = 1;
            perm[depth] = v;
            gen_perms_rec(depth + 1, n, perm, used, cnt);
            used[v] = 0;
        }
    }
}

static void compute_all_counts(seq_t *out) {
    int perm[9];
    char used[9];
    for (int n = 0; n <= 9; ++n) {
        out[n] = 0;
        if (n == 0) {
            out[0] = 1;
            continue;
        }
        for (int i = 0; i < 9; ++i) used[i] = 0;
        gen_perms_rec(0, n, perm, used, &out[n]);
    }
}

/* This function counts permutations of [n] avoiding the patterns
   4321, 4123, 1234.  For n ≤ 9 it enumerates all permutations by
   recursion; for n ≥ 10 it returns 0 since any permutation of length
   10 or more necessarily contains an increasing or decreasing
   subsequence of length 4.  The enumeration is performed only once
   for all n ≤ 9 and the results are cached. */

seq_t a(idx_t n) {
    static bool table_ready = false;
    static seq_t table[10];
    if (n < 10) {
        if (!table_ready) {
            compute_all_counts(table);
            table_ready = true;
        }
        return table[n];
    }
    return 0;
}
