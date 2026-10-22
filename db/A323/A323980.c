/*
 * A323980  Expansion of determinant of 24 X 24 matrix M_{u,v} =
 *          q^(inv(u*v^-1)) where u, v in S_4 and inv is number of inversions.
 * https://oeis.org/A323980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of q^n in (1-q)^36*(1-q^3)^8*(1-q^6)^2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static seq_t binom36[37];
    static seq_t binom8[9];
    static seq_t binom2[3];
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 0; i <= 36; ++i)
            binom36[i] = binom((idx_t)36, i);
        for (idx_t j = 0; j <= 8; ++j)
            binom8[j] = binom((idx_t)8, j);
        binom2[0] = 1;          /* C(2,0) */
        binom2[1] = 2;          /* C(2,1) */
        binom2[2] = 1;          /* C(2,2) */
        initialized = true;
    }

    if (n > 72) return 0;          /* beyond highest degree */

    seq_t result = 0;

    for (idx_t i = 0; i <= 36 && i <= n; ++i) {
        seq_t bi = binom36[i];
        idx_t rem1 = n - i;

        /* Solve 3*j + 6*k = rem1  with 0 <= j <= 8, 0 <= k <= 2 */
        for (idx_t j = 0; j <= 8 && 3 * j <= rem1; ++j) {
            idx_t rem2 = rem1 - 3 * j;
            if (rem2 % 6 != 0) continue;
            idx_t k = rem2 / 6;
            if (k > 2) continue;

            seq_t prod = bi * binom8[j] * binom2[k];
            if ((i + j + k) & 1U)   /* odd number of minus signs: subtract */
                result -= prod;
            else                  /* even number: add */
                result += prod;
        }
    }

    return result;
}
