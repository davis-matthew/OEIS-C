/*
 * A251330  Number of (n+1)X(3+1) 0..2 arrays with every 2X2 subblock summing
 *          to 2 4 or 6.
 * https://oeis.org/A251330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with entries 0..2 where every
 *            2×2 subblock sums to 2, 4, or 6. Computed by a 81×81 adjacency
 *            matrix exponentiation (row patterns over {0,1,2})
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 201 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static void init_adj_matrix(seq_t *M)
{
    /* All 81 patterns of length 4 over {0,1,2}. */
    int pat[81][4];
    for (int idx = 0; idx < 81; ++idx) {
        int v = idx;
        for (int d = 0; d < 4; ++d) {
            pat[idx][d] = v % 3;
            v /= 3;
        }
    }

    /* Build adjacency matrix M[i][j] = 1 if rows i and j can be adjacent,
       otherwise 0. */
    for (int i = 0; i < 81; ++i) {
        for (int j = 0; j < 81; ++j) {
            int ok = 1;
            for (int c = 0; c < 3; ++c) {
                int s = pat[i][c] + pat[i][c+1] +
                        pat[j][c] + pat[j][c+1];
                if (!(s == 2 || s == 4 || s == 6)) {
                    ok = 0;
                    break;
                }
            }
            M[i * 81 + j] = ok ? (seq_t)1 : (seq_t)0;
        }
    }
}

/* vector * matrix (row–major).  Result stored in out. */
static void vec_mul_mat(const seq_t *vec, const seq_t *mat, seq_t *out)
{
    for (int j = 0; j < 81; ++j) out[j] = 0;
    for (int i = 0; i < 81; ++i) {
        if (vec[i] == 0) continue;
        for (int j = 0; j < 81; ++j) {
            out[j] += vec[i] * mat[i * 81 + j];
        }
    }
}

/* matrix multiplication: C = A * B (row–major).  Result stored in C. */
static void mat_mul(const seq_t *A, const seq_t *B, seq_t *C)
{
    seq_t tmp[81 * 81];
    for (int i = 0; i < 81 * 81; ++i) tmp[i] = 0;
    for (int i = 0; i < 81; ++i) {
        for (int k = 0; k < 81; ++k) {
            if (A[i * 81 + k] == 0) continue;
            for (int j = 0; j < 81; ++j) {
                tmp[i * 81 + j] += A[i * 81 + k] * B[k * 81 + j];
            }
        }
    }
    for (int i = 0; i < 81 * 81; ++i) C[i] = tmp[i];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 */

    /* Adjacency matrix A (81×81). */
    static seq_t A[81 * 81];
    static int init = 0;
    if (!init) {
        init_adj_matrix(A);
        init = 1;
    }

    /* Initial vector of all ones (first row patterns). */
    seq_t v[81], vtmp[81];
    for (int i = 0; i < 81; ++i) v[i] = 1;

    /* Current matrix power (start with A). */
    seq_t M[81 * 81];
    for (int i = 0; i < 81 * 81; ++i) M[i] = A[i];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            vec_mul_mat(v, M, vtmp);
            for (int i = 0; i < 81; ++i) v[i] = vtmp[i];
        }
        /* M = M * M */
        seq_t M2[81 * 81];
        mat_mul(M, M, M2);
        for (int i = 0; i < 81 * 81; ++i) M[i] = M2[i];
        exp >>= 1;
    }

    /* Sum all entries of v to get a(n). */
    seq_t res = 0;
    for (int i = 0; i < 81; ++i) res += v[i];
    return res;
}
