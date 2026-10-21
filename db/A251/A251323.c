/*
 * A251323  Number of (n+1)X(5+1) 0..1 arrays with every 2X2 subblock having
 *          one or two 1s.
 * https://oeis.org/A251323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 binary arrays such that every 2×2
 *            subblock has exactly 1 or 2 ones. Computation uses
 *            transfer-matrix exponentiation over a 64×64 adjacency matrix
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   vecmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/vecmul.h"

static seq_t A_mat[64][64];          /* adjacency matrix (once per program) */
static bool A_initialized = false;   /* flag to build A_mat only once */

/* Build adjacency matrix: A[i][j] = 1 if row i can be followed by row j, else 0. */
static void build_adjacency(void)
{
    if (A_initialized) return;
    for (idx_t r = 0; r < 64; ++r) {
        for (idx_t s = 0; s < 64; ++s) {
            bool ok = true;
            for (idx_t j = 0; j < 5; ++j) {  /* 5 positions for 2x2 blocks in width 6 */
                int r1 = (r >> j) & 1;
                int r2 = (r >> (j + 1)) & 1;
                int s1 = (s >> j) & 1;
                int s2 = (s >> (j + 1)) & 1;
                int sum = r1 + r2 + s1 + s2;
                if (sum != 1 && sum != 2) { ok = false; break; }
            }
            A_mat[r][s] = ok ? 1 : 0;
        }
    }
    A_initialized = true;
}

/* Multiply two 64×64 matrices: res = M1 × M2 */
static void matmul(seq_t res[64][64], const seq_t M1[64][64], const seq_t M2[64][64])
{
    seq_t tmp[64][64];
    /* initialize tmp to zero */
    for (idx_t i = 0; i < 64; ++i)
        for (idx_t j = 0; j < 64; ++j)
            tmp[i][j] = 0;

    for (idx_t i = 0; i < 64; ++i) {
        for (idx_t k = 0; k < 64; ++k) {
            if (M1[i][k] == 0) continue;
            for (idx_t j = 0; j < 64; ++j) {
                tmp[i][j] += M1[i][k] * M2[k][j];
            }
        }
    }

    /* copy tmp back to res */
    for (idx_t i = 0; i < 64; ++i)
        for (idx_t j = 0; j < 64; ++j)
            res[i][j] = tmp[i][j];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    build_adjacency();

    /* We need A^n (where n = number of transitions, i.e., floor(n)) */
    idx_t exp = n;               /* n ≥ 1 by specification */

    /* Working copy of the matrix for exponentiation */
    seq_t M_pow[64][64];
    for (idx_t i = 0; i < 64; ++i)
        for (idx_t j = 0; j < 64; ++j)
            M_pow[i][j] = A_mat[i][j];

    /* v = all-ones row vector */
    seq_t v[64];
    for (idx_t i = 0; i < 64; ++i) v[i] = 1;

    seq_t v_tmp[64];

    while (exp > 0) {
        if (exp & 1ULL) {
            vecmul(v_tmp, v, M_pow);
            for (idx_t i = 0; i < 64; ++i) v[i] = v_tmp[i];
        }
        seq_t M_next[64][64];
        matmul(M_next, M_pow, M_pow);
        for (idx_t i = 0; i < 64; ++i)
            for (idx_t j = 0; j < 64; ++j)
                M_pow[i][j] = M_next[i][j];
        exp >>= 1ULL;
    }

    /* Sum entries of the final vector */
    seq_t ans = 0;
    for (idx_t i = 0; i < 64; ++i) ans += v[i];
    return ans;
}
