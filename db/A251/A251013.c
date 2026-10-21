/*
 * A251013  Number of (n+1)X(2+1) 0..2 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2} with no 2×2
 *            subblock having the sum of its diagonal elements less than the
 *            maximum of its antidiagonal elements. Computation: an adjacency
 *            automaton on the 27 possible row patterns; a(1)=423, a(2)=6653,
 *            … (see OEIS A251013)
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Total number of row patterns for 3 columns with entries 0..2. */
    enum { TOT = 27 };

    /* Lazy static initialization of the state transition matrix. */
    static int initialized = 0;
    static seq_t M[TOT][TOT];
    static int pat[TOT][3];

    if (!initialized) {
        /* Pre‑compute base‑3 digits for each pattern. */
        for (idx_t p = 0; p < TOT; ++p) {
            idx_t x = p;
            for (int c = 0; c < 3; ++c) {
                pat[p][c] = (int)(x % 3);   /* column c (0‑based) */
                x /= 3;
            }
        }

        /* Build the adjacency matrix.
           M[i][j] = 1 iff a row with pattern i can be followed by
           a row with pattern j without violating the 2×2 rule. */
        for (idx_t i = 0; i < TOT; ++i) {
            for (idx_t j = 0; j < TOT; ++j) {
                bool ok = true;
                for (int c = 0; c < 2; ++c) {
                    int a = pat[i][c];
                    int b = pat[i][c + 1];
                    int d = pat[j][c];
                    int e = pat[j][c + 1];
                    /* Check: a + e (main diagonal) >= max(b, c) (anti‑diagonal). */
                    int max_ac = (b > d) ? b : d;
                    if (a + e < max_ac) {
                        ok = false;
                        break;
                    }
                }
                M[i][j] = ok ? (seq_t)1 : (seq_t)0;
            }
        }
        initialized = 1;
    }

    if (n == 0) return 0;   /* Sequence is not defined for n < 1. */

    /* Current vector of counts for each ending row after processed
       transitions.  Start with a single arbitrary row. */
    seq_t vec[TOT];
    seq_t tmpVec[TOT];
    for (idx_t i = 0; i < TOT; ++i) vec[i] = (seq_t)1;

    /* Working copy of M for exponentiation. */
    seq_t curM[TOT][TOT];
    seq_t nextM[TOT][TOT];
    for (idx_t i = 0; i < TOT; ++i)
        for (idx_t j = 0; j < TOT; ++j)
            curM[i][j] = M[i][j];

    idx_t power = n;   /* number of transitions between rows = n. */
    while (power > 0) {
        if (power & 1) {
            /* vec = curM * vec (vector right‑multiplication). */
            for (idx_t j = 0; j < TOT; ++j) {
                seq_t sum = (seq_t)0;
                for (idx_t i = 0; i < TOT; ++i) {
                    sum += vec[i] * curM[i][j];
                }
                tmpVec[j] = sum;
            }
            for (idx_t i = 0; i < TOT; ++i) vec[i] = tmpVec[i];
        }

        /* curM = curM * curM (matrix squaring). */
        for (idx_t i = 0; i < TOT; ++i) {
            for (idx_t j = 0; j < TOT; ++j) {
                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k < TOT; ++k) {
                    sum += curM[i][k] * curM[k][j];
                }
                nextM[i][j] = sum;
            }
        }
        for (idx_t i = 0; i < TOT; ++i)
            for (idx_t j = 0; j < TOT; ++j)
                curM[i][j] = nextM[i][j];

        power >>= 1;
    }

    /* Sum over all possible final rows to obtain the total number of arrays. */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < TOT; ++i) result += vec[i];
    return result;
}
