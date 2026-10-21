/*
 * A251097  Number of (n+1)X(5+1) 0..1 arrays with no 2X2 subblock having its
 *          minimum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A251097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i,j=0}^{63} (T^n)[i][j] where T[i][j] is 1 if row
 *            pattern i can be followed by row pattern j under the 2x2
 *            subblock constraint. This equals the number of walks of length n
 *            in the 64‑state graph
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compatibility matrix – 64 states (6‑bit row patterns). */
static bool compat[64][64];
static bool init_compat = false;

/* Build compatibility matrix once. */
static void build_compat(void)
{
    for (int p = 0; p < 64; ++p) {
        for (int q = 0; q < 64; ++q) {
            bool ok = true;
            for (int j = 0; j < 5; ++j) {
                int a = (p >> j) & 1;          /* top left   */
                int b = (p >> (j + 1)) & 1;    /* top right  */
                int c = (q >> j) & 1;          /* bottom left*/
                int d = (q >> (j + 1)) & 1;    /* bottom right*/
                if (b && c && (!a || !d)) {    /* forbidden pattern */
                    ok = false;
                    break;
                }
            }
            compat[p][q] = ok;
        }
    }
}

/* Matrix multiplication: R = A * B (size 64x64). */
static void mat_mul(const seq_t A[64][64], const seq_t B[64][64], seq_t R[64][64])
{
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 64; ++k)
                sum += A[i][k] * B[k][j];
            R[i][j] = sum;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!init_compat) {
        build_compat();
        init_compat = true;
    }

    /* v[i] = number of ways to reach state i after current number of steps. */
    seq_t v[64];
    seq_t nv[64];
    for (int i = 0; i < 64; ++i)
        v[i] = (seq_t)1;           /* start: any row pattern is possible */

    /* M = adjacency matrix (64x64). */
    seq_t M[64][64];
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            M[i][j] = compat[i][j] ? (seq_t)1 : (seq_t)0;

    idx_t exp = n;   /* number of transitions (rows - 1) */

    seq_t Mtmp[64][64];

    while (exp) {
        if (exp & 1) {
            /* v = v * M */
            for (int j = 0; j < 64; ++j)
                nv[j] = 0;
            for (int i = 0; i < 64; ++i) {
                if (!v[i]) continue;
                for (int j = 0; j < 64; ++j) {
                    if (!M[i][j]) continue;
                    nv[j] += v[i] * M[i][j];
                }
            }
            for (int i = 0; i < 64; ++i)
                v[i] = nv[i];
        }

        /* M = M * M */
        mat_mul(M, M, Mtmp);
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                M[i][j] = Mtmp[i][j];

        exp >>= 1;
    }

    /* Sum over all ending states. */
    seq_t sum = 0;
    for (int i = 0; i < 64; ++i)
        sum += v[i];

    return sum;
}
