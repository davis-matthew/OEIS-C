/*
 * A254611  Number of perfect matchings in the P_6 X C_n graph.
 * https://oeis.org/A254611
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = trace(M^n) where M is the 64×64 transfer matrix for the
 *            graph P_6 × C_n (perfect matchings on a cylindrical 6×n grid)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 25 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static void copy_mat(const seq_t src[64][64], seq_t dst[64][64])
{
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dst[i][j] = src[i][j];
}

static void dfs_build(int s, int cur, int nxt, seq_t *row)
{
    if (cur == 0x3F) {                 /* all six vertices matched */
        row[nxt] += 1;
        return;
    }
    /* find first free vertex in this column */
    int pos = 0;
    while (cur & (1 << pos)) pos++;

    /* horizontal match to the next column */
    dfs_build(s, cur | (1 << pos), nxt | (1 << pos), row);

    /* vertical match with the next row, if possible */
    if (pos + 1 < 6 && !(cur & (1 << (pos + 1)))) {
        dfs_build(s, cur | (1 << pos) | (1 << (pos + 1)), nxt, row);
    }
}

/* ---------- matrix operations on 64×64 matrices ---------- */
static void matmul(const seq_t A[64][64], const seq_t B[64][64],
                   seq_t C[64][64])
{
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 64; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* ---------- build the 64×64 transfer matrix ---------- */
static seq_t M[64][64];
static bool made_matrix = false;

static void make_matrix(void)
{
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            M[i][j] = 0;

    for (int s = 0; s < 64; ++s)
        dfs_build(s, s, 0, M[s]);

    made_matrix = true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    if (!made_matrix)
        make_matrix();

    /* fast exponentiation of the matrix M */
    seq_t R[64][64];       /* result matrix (starts as identity) */
    seq_t A[64][64];       /* current power of M */
    seq_t tmp[64][64];     /* temporaries for multiplication */

    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            R[i][j] = (i == j) ? 1 : 0;

    copy_mat(M, A);

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            matmul(R, A, tmp);
            copy_mat(tmp, R);
        }
        exp >>= 1;
        if (exp == 0)
            break;
        matmul(A, A, tmp);
        copy_mat(tmp, A);
    }

    /* trace of the resulting matrix is the desired count */
    seq_t ans = 0;
    for (int i = 0; i < 64; ++i)
        ans += R[i][i];

    return ans;
}
