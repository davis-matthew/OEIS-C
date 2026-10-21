/*
 * A251253  Number of (n+1) X (3+1) 0..1 arrays with every 2 X 2 subblock
 *          having a single 1 or two 1s on the same edge.
 * https://oeis.org/A251253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all entries of M^n where M is the 16×16 adjacency
 *            matrix of compatible 4‑bit rows (rows r and s are compatible iff
 *            each of the three 2×2 subblocks formed by
 *            r[j],r[j+1],s[j],s[j+1] has either exactly one '1' or exactly
 *            two '1's that are adjacent). This gives a(1)=38, a(2)=150, etc
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* adjacency matrix M[16][16] */
    static seq_t M[16][16];
    static bool initialized = false;

    if (!initialized) {
        /* pre‑compute M */
        int r, s, j;
        int bit_r[16][4], bit_s[16][4];
        for (r = 0; r < 16; ++r) {
            for (j = 0; j < 4; ++j)
                bit_r[r][j] = (r >> j) & 1;
        }
        for (s = 0; s < 16; ++s) {
            for (j = 0; j < 4; ++j)
                bit_s[s][j] = (s >> j) & 1;
        }
        for (r = 0; r < 16; ++r) {
            for (s = 0; s < 16; ++s) {
                int ok = 1;
                for (j = 0; j < 3; ++j) {
                    int a = bit_r[r][j];
                    int b = bit_r[r][j+1];
                    int c = bit_s[s][j];
                    int d = bit_s[s][j+1];
                    int cnt = a + b + c + d;
                    int block_ok = 0;
                    if (cnt == 1) {
                        block_ok = 1;
                    } else if (cnt == 2) {
                        /* adjacent 2 ones */
                        if ((a && b && !c && !d) ||
                            (!a && !b && c && d) ||
                            (a && c && !b && !d) ||
                            (!a && !c && b && d))
                            block_ok = 1;
                    }
                    if (!block_ok) {
                        ok = 0;
                        break;
                    }
                }
                M[r][s] = ok ? (seq_t)1 : (seq_t)0;
            }
        }
        initialized = true;
    }

    /* Fast exponentiation of M to the n‑th power */
    seq_t R[16][16], base[16][16], tmp[16][16];
    int i, j, k;
    /* identity matrix */
    for (i = 0; i < 16; ++i)
        for (j = 0; j < 16; ++j)
            R[i][j] = (seq_t)((i == j) ? 1 : 0);
    /* base = M */
    for (i = 0; i < 16; ++i)
        for (j = 0; j < 16; ++j)
            base[i][j] = M[i][j];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            /* R = R * base */
            for (i = 0; i < 16; ++i)
                for (j = 0; j < 16; ++j) {
                    seq_t sum = (seq_t)0;
                    for (k = 0; k < 16; ++k)
                        sum += R[i][k] * base[k][j];
                    tmp[i][j] = sum;
                }
            for (i = 0; i < 16; ++i)
                for (j = 0; j < 16; ++j)
                    R[i][j] = tmp[i][j];
        }
        /* base = base * base */
        for (i = 0; i < 16; ++i)
            for (j = 0; j < 16; ++j) {
                seq_t sum = (seq_t)0;
                for (k = 0; k < 16; ++k)
                    sum += base[i][k] * base[k][j];
                tmp[i][j] = sum;
            }
        for (i = 0; i < 16; ++i)
            for (j = 0; j < 16; ++j)
                base[i][j] = tmp[i][j];
        exp >>= 1;
    }

    /* sum all entries of R */
    seq_t total = (seq_t)0;
    for (i = 0; i < 16; ++i)
        for (j = 0; j < 16; ++j)
            total += R[i][j];
    return total;
}
