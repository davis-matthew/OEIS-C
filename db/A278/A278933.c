/*
 * A278933  Number of 2 X 2 matrices with entries in {0,1,...,n} and permanent
 *          = trace with no entry repeated.
 * https://oeis.org/A278933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×2 matrices with entries in {0,1,…,n} whose
 *            permanent equals its trace and whose four entries are distinct
 * Range:     u8 n <= 59, u16 n <= 4929, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   forbidden_pairs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/forbidden_pairs.h"

seq_t a(idx_t n)
{
    /* Pre‑compute the number of distinct ordered pairs (b,c)
       with product R (0 ≤ R ≤ n). */
    size_t N = (size_t)n + 1;
    seq_t *T = (seq_t *)malloc(N * sizeof(seq_t));
    if (!T) return (seq_t)0;                     /* allocation failure */
    T[0] = (seq_t)2 * (seq_t)n;                  /* pairs with product 0 */

    for (idx_t R = 1; R <= n; ++R) {
        seq_t ct = 0;
        for (idx_t d = 1; d * d <= R; ++d) {
            if (R % d == 0) {
                idx_t e = R / d;
                if (d < e && d <= n && e <= n) ct += 2; /* (d,e) and (e,d) */
            }
        }
        T[R] = ct;
    }

    seq_t result = 0;
    int maxX = (int)n - 1;   /* X ranges from –1 to n–1 (inclusive) */

    for (int X = -1; X <= maxX; ++X) {
        idx_t a_val = (idx_t)X + 1;          /* a = X+1 */

        if (X == -1) {
            for (int Y = -1; Y <= (int)n - 1; ++Y) {
                if (Y == -1) continue;     /* require a ≠ d */
                idx_t d_val = (idx_t)Y + 1;
                int64_t prod = (int64_t)X * (int64_t)Y;
                int64_t R_signed = 1 - prod;
                if (R_signed < 0 || (idx_t)R_signed > n) continue;
                idx_t R = (idx_t)R_signed;

                seq_t T_R = T[R];
                seq_t Va = forbidden_pairs(a_val, R, n);
                seq_t Vd = forbidden_pairs(d_val, R, n);
                seq_t bad = Va + Vd;
                if (a_val != d_val && a_val * d_val == R) bad -= 2;
                if (bad > T_R) bad = T_R;
                result += T_R - bad;
            }
        } else if (X == 0) {
            for (int Y = -1; Y <= (int)n - 1; ++Y) {
                if (Y == 0) continue;
                idx_t d_val = (idx_t)Y + 1;
                int64_t prod = (int64_t)X * (int64_t)Y;
                int64_t R_signed = 1 - prod;
                if (R_signed < 0 || (idx_t)R_signed > n) continue;
                idx_t R = (idx_t)R_signed;

                seq_t T_R = T[R];
                seq_t Va = forbidden_pairs(a_val, R, n);
                seq_t Vd = forbidden_pairs(d_val, R, n);
                seq_t bad = Va + Vd;
                if (a_val != d_val && a_val * d_val == R) bad -= 2;
                if (bad > T_R) bad = T_R;
                result += T_R - bad;
            }
        } else if (X == 1) {
            int Ycand[2] = { -1, 0 };
            for (int yi = 0; yi < 2; ++yi) {
                int Y = Ycand[yi];
                idx_t d_val = (idx_t)Y + 1;
                int64_t prod = (int64_t)X * (int64_t)Y;
                int64_t R_signed = 1 - prod;
                if (R_signed < 0 || (idx_t)R_signed > n) continue;
                idx_t R = (idx_t)R_signed;

                seq_t T_R = T[R];
                seq_t Va = forbidden_pairs(a_val, R, n);
                seq_t Vd = forbidden_pairs(d_val, R, n);
                seq_t bad = Va + Vd;
                if (a_val != d_val && a_val * d_val == R) bad -= 2;
                if (bad > T_R) bad = T_R;
                result += T_R - bad;
            }
        } else { /* X >= 2 */
            int Ycand[2] = { -1, 0 };
            for (int yi = 0; yi < 2; ++yi) {
                int Y = Ycand[yi];
                idx_t d_val = (idx_t)Y + 1;
                int64_t prod = (int64_t)X * (int64_t)Y;
                int64_t R_signed = 1 - prod;
                if (R_signed < 0 || (idx_t)R_signed > n) continue;
                idx_t R = (idx_t)R_signed;

                seq_t T_R = T[R];
                seq_t Va = forbidden_pairs(a_val, R, n);
                seq_t Vd = forbidden_pairs(d_val, R, n);
                seq_t bad = Va + Vd;
                if (a_val != d_val && a_val * d_val == R) bad -= 2;
                if (bad > T_R) bad = T_R;
                result += T_R - bad;
            }
        }
    }

    free(T);
    return result;
}
