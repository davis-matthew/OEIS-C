/*
 * A250970  Number of (n+1)X(5+1) 0..1 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the absolute difference of
 *          its antidiagonal elements.
 * https://oeis.org/A250970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i,j} (M^n)[i][j], * where M is the 64×64 adjacency
 *            matrix for 6-bit rows. * M[u][v] = 1 if and only if for all 0 ≤
 *            k < 5 * (bit(u,k) + bit(v,k+1)) ≥ |bit(u,k+1) – bit(v,k)|. * The
 *            sequence counts (n+1)×6 binary arrays with the given 2×2 block *
 *            restriction. It is computed by transfer–...
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bits.h"

#define STATES 64  /* 6-bit rows */

/* build the adjacency matrix the first time a() is called */
static void build_matrix(seq_t M[STATES][STATES]) {
    int u, v, k;
    for (u = 0; u < STATES; ++u) {
        for (v = 0; v < STATES; ++v) {
            int ok = 1;
            for (k = 0; k < 5; ++k) {
                int a = bits(u, k);
                int b = bits(v, k);
                int c = bits(u, k + 1);
                int d = bits(v, k + 1);
                int diff = c - b;
                if (diff < 0) diff = -diff;
                if (a + d < diff) { ok = 0; break; }
            }
            M[u][v] = ok ? 1 : 0;
        }
    }
}

/* matrix multiplication: C = A * B */
static void mat_mul(const seq_t A[STATES][STATES],
                    const seq_t B[STATES][STATES],
                    seq_t C[STATES][STATES]) {
    int i, j, k;
    seq_t temp[STATES][STATES] = {0};
    for (i = 0; i < STATES; ++i) {
        for (k = 0; k < STATES; ++k) {
            if (A[i][k] == 0) continue;
            seq_t aik = A[i][k];
            for (j = 0; j < STATES; ++j) {
                temp[i][j] += aik * B[k][j];
            }
        }
    }
    for (i = 0; i < STATES; ++i)
        for (j = 0; j < STATES; ++j)
            C[i][j] = temp[i][j];
}

/* power: result = base^exp (exp ≥ 0) */
static void mat_pow(const seq_t base[STATES][STATES],
                    seq_t result[STATES][STATES],
                    idx_t exp) {
    int i, j;
    /* initialize result as identity matrix */
    for (i = 0; i < STATES; ++i) {
        for (j = 0; j < STATES; ++j) {
            result[i][j] = (i == j) ? 1 : 0;
        }
    }
    /* temporary matrix for squaring */
    seq_t temp[STATES][STATES];
    seq_t cur[STATES][STATES];
    /* copy base into cur */
    for (i = 0; i < STATES; ++i)
        for (j = 0; j < STATES; ++j)
            cur[i][j] = base[i][j];

    idx_t e = exp;
    while (e > 0) {
        if (e & 1) {
            mat_mul(result, cur, temp);
            /* copy temp to result */
            for (i = 0; i < STATES; ++i)
                for (j = 0; j < STATES; ++j)
                    result[i][j] = temp[i][j];
        }
        e >>= 1;
        if (e) { /* square cur only if needed */
            mat_mul(cur, cur, temp);
            for (i = 0; i < STATES; ++i)
                for (j = 0; j < STATES; ++j)
                    cur[i][j] = temp[i][j];
        }
    }
}

/* main sequence routine */
seq_t a(idx_t n) {
    static int initialized = 0;
    static seq_t matrix[STATES][STATES];

    if (!initialized) {
        build_matrix(matrix);
        initialized = 1;
    }

    /* n is the number of transitions (rows-1) */
    seq_t power[STATES][STATES];
    mat_pow(matrix, power, n);

    /* sum all entries */
    seq_t total = 0;
    int i, j;
    for (i = 0; i < STATES; ++i)
        for (j = 0; j < STATES; ++j)
            total += power[i][j];

    return total;
}
