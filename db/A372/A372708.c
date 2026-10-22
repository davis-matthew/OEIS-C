/*
 * A372708  a(n) is the smallest number k that is the concatenation of the
 *          elements of a 3 X 3 matrix whose determinant is n and whose
 *          elements are a permutation of the numbers 1 through 9; a(n) = -1
 *          if no such number k exists.
 * https://oeis.org/A372708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, fini, nonn, less
 * Formula:   The term a(n) is the minimal 9‑digit number that can be formed *
 *            by concatenating the elements of a 3×3 matrix whose entries are
 *            a * permutation of 1…9 and whose determinant equals n. If no
 *            such matrix * exists (in particular for n > 412), a(n) = -1. *
 *            The function computes all 9! permutations on...
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int next_perm(int *a, int n) {
    int i = n - 2;
    while (i >= 0 && a[i] > a[i + 1]) --i;
    if (i < 0) return 0;
    int j = n - 1;
    while (a[j] < a[i]) --j;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int l = i + 1, r = n - 1; l < r; ++l, --r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return 1;
}

seq_t a(idx_t n) {
    /* Memoisation data, built on first call. */
    static bool built = false;
    static seq_t mink[413];
    static bool has[413];

    if (!built) {
        /* initialise arrays */
        for (int i = 0; i < 413; ++i) has[i] = false;

        int d[9] = {1,2,3,4,5,6,7,8,9};
        do {
            int det = d[0]*(d[4]*d[8]-d[5]*d[7]) -
                      d[1]*(d[3]*d[8]-d[5]*d[6]) +
                      d[2]*(d[3]*d[7]-d[4]*d[6]);
            if (det >= 0 && det <= 412) {
                seq_t k = 0;
                for (int i = 0; i < 9; ++i) k = k*10 + d[i];
                if (!has[det] || k < mink[det]) {
                    has[det] = true;
                    mink[det] = k;
                }
            }
        } while (next_perm(d, 9));
        built = true;
    }

    if (n >= 413) return (seq_t)-1;
    if (!has[(int)n]) return (seq_t)-1;
    return mink[(int)n];
}
