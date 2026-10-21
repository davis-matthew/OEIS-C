/*
 * A250959  Number of (n+1)X(2+1) 0..3 arrays with no 2X2 subblock having its
 *          minimum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A250959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250959: Number of (n+1)×3 0..3 arrays with no 2×2 subblock having its minimum diagonal element less than its minimum antidiagonal element.
   Computation is performed via a 64×64 transition matrix A where
   A[i][j] = 1 iff rows i and j are compatible.
   The number of arrays of height n+1 is 1^T · A^n · 1, computed
   by exponentiating A with binary exponentiation while applying
   the accumulated matrix directly to a vector of all ones.  */

/* ---------- include for bool --------------------------------------- */
#include <stdbool.h>

/* ---------- global transition matrix -------------------------------- */
static seq_t A[64][64];
static bool initialized = false;

/* ---------- helper: multiply two 64×64 matrices --------------------- */
/* Result matrix C = M1 * M2 (mod seq_t).  All loops use indices 0..63. */
static void mat_mul(const seq_t (*M1)[64], const seq_t (*M2)[64],
                    seq_t (*C)[64]) {
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 64; ++k) {
                sum += M1[i][k] * M2[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* ---------- helper: multiply vector by matrix ---------------------- */
/* Returns vec' = vec * M, where vec is a row vector of length 64.    */
static void vec_mat_mul(const seq_t vec[64], const seq_t (*M)[64],
                        seq_t res[64]) {
    for (int j = 0; j < 64; ++j) {
        seq_t sum = 0;
        for (int i = 0; i < 64; ++i) {
            sum += vec[i] * M[i][j];
        }
        res[j] = sum;
    }
}

/* ---------- initialise transition matrix A ----------------------- */
static void init_matrix(void) {
    for (idx_t i = 0; i < 64; ++i) {
        int p0 = i & 3;
        int p1 = (i >> 2) & 3;
        int p2 = (i >> 4) & 3;
        for (idx_t j = 0; j < 64; ++j) {
            int q0 = j & 3;
            int q1 = (j >> 2) & 3;
            int q2 = (j >> 4) & 3;

            int min_diag1 = (p0 < q1) ? p0 : q1;
            int min_ant1  = (p1 < q0) ? p1 : q0;
            int min_diag2 = (p1 < q2) ? p1 : q2;
            int min_ant2  = (p2 < q1) ? p2 : q1;

            A[i][j] = (seq_t)((min_diag1 >= min_ant1) &&
                               (min_diag2 >= min_ant2));
        }
    }
    initialized = true;
}

/* ---------- main sequence function --------------------------------- */
seq_t a(idx_t n) {
    if (!initialized) init_matrix();

    /* vector of ones */
    seq_t vec[64];
    for (int i = 0; i < 64; ++i) vec[i] = 1;

    /* matrix power exponentiation with vector accumulation */
    seq_t curr[64][64];
    seq_t next[64];      /* temporary vector */

    /* copy base matrix to curr */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            curr[i][j] = A[i][j];

    idx_t exp = n;       /* n >= 1 */

    while (exp) {
        if (exp & 1ULL) {
            /* vec = vec * curr */
            vec_mat_mul(vec, curr, next);
            for (int i = 0; i < 64; ++i) vec[i] = next[i];
        }
        /* curr = curr * curr */
        seq_t temp[64][64];
        mat_mul(curr, curr, temp);
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                curr[i][j] = temp[i][j];

        exp >>= 1ULL;
    }

    /* sum over all ending rows */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i) total += vec[i];
    return total;
}
