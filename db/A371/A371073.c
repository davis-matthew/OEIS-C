/*
 * A371073  a(n)/144 is the maximum squared volume of a tetrahedron with the
 *          sum of its integer edge lengths equal to n.
 * https://oeis.org/A371073
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = "maximum squared volume of a tetrahedron with integer
 *            edge lengths summing to n" multiplied by 144, i.e. a(n) =
 *            det(M)/2 where M is the Cayley–Menger matrix
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   valid_triangles
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_triangles.h"

/* Permutations of 0..4 used for the 5x5 determinant (120 elements). */
static int perm5[120][5];
static int sign5[120];
static int perm5_cnt = 0;

/* Recursive generation of all permutations of 0..4. */
static void perm5_recursive(int depth, int cur[5], bool used[5])
{
    if (depth == 5) {
        for (int i = 0; i < 5; ++i)
            perm5[perm5_cnt][i] = cur[i];
        int inv = 0;
        for (int a = 0; a < 5; ++a)
            for (int b = a + 1; b < 5; ++b)
                if (cur[a] > cur[b]) ++inv;
        sign5[perm5_cnt] = (inv & 1) ? -1 : 1;   /* parity */
        ++perm5_cnt;
        return;
    }
    for (int i = 0; i < 5; ++i)
        if (!used[i]) {
            used[i] = true;
            cur[depth] = i;
            perm5_recursive(depth + 1, cur, used);
            used[i] = false;
        }
}

/* Initialise the permutation table once. */
static void init_perm5(void)
{
    if (perm5_cnt == 0) {
        int cur[5];
        bool used[5] = {0};
        perm5_recursive(0, cur, used);
    }
}

/* Compute the Cayley–Menger determinant for a single edge assignment. */
static __int128 det5_from_edges(const int e[6])
{
    init_perm5();

    __int128 m[5][5];
    /* first row & column */
    m[0][0] = 0;
    for (int j = 1; j < 5; ++j) m[0][j] = m[j][0] = 1;

    /* squared edges */
    __int128 u2 = (__int128)e[0] * e[0];
    __int128 v2 = (__int128)e[1] * e[1];
    __int128 w2 = (__int128)e[2] * e[2];
    __int128 U2 = (__int128)e[3] * e[3];
    __int128 V2 = (__int128)e[4] * e[4];
    __int128 W2 = (__int128)e[5] * e[5];

    /* fill the rest of the matrix */
    m[1][1] = 0;  m[1][2] = u2; m[1][3] = v2; m[1][4] = w2;
    m[2][1] = u2; m[2][2] = 0;  m[2][3] = W2; m[2][4] = V2;
    m[3][1] = v2; m[3][2] = W2; m[3][3] = 0;  m[3][4] = U2;
    m[4][1] = w2; m[4][2] = V2; m[4][3] = U2; m[4][4] = 0;

    __int128 det = 0;
    for (int p = 0; p < perm5_cnt; ++p) {
        __int128 prod = m[0][perm5[p][0]];
        for (int i = 1; i < 5; ++i)
            prod *= m[i][perm5[p][i]];
        det += sign5[p] * prod;
    }
    return det;
}

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 9; values below 9 are irrelevant. */
    if (n < 9) return 0;

    int e = (int)(n / 6);      /* base edge length */
    int r = (int)(n % 6);      /* number of edges that are e+1 */

    seq_t best = 0;

    /* iterate over all 2^6 = 64 masks; keep those with exactly r bits set. */
    for (int mask = 0; mask < 64; ++mask) {
        if (__builtin_popcount((unsigned)mask) != r) continue;

        /* Assign edges according to mask: bit i set ⇒ edge = e+1. */
        int edge[6];
        for (int i = 0; i < 6; ++i)
            edge[i] = (mask & (1 << i)) ? e + 1 : e;

        /* Quick filter by triangle inequalities. */
        if (!valid_triangles(edge)) continue;

        /* Compute Cayley–Menger determinant. */
        __int128 det = det5_from_edges(edge);
        if (det <= 0) continue;   /* non‑degenerate tetrahedron only */

        seq_t val = (seq_t)(det / 2);   /* a(n) = det / 2 */
        if (val > best) best = val;
    }

    return best;
}
