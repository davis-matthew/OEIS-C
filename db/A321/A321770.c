/*
 * A321770  Consider the tree of triples P(n, k) with n > 0 and 0 < k <=
 *          3^(n-1), such that P(1, 1) = [3; 4; 5] and each triple t on some
 *          row branches to the triples A*t, B*t, C*t on the next row (with A
 *          = [1, -2, 2; 2, -1, 2; 2, -2, 3], B = [1, 2, 2; 2, 1, 2; 2, 2, 3]
 *          and C = [-1, 2, 2; -2, 1, 2; -2, 2, 3])...
 * https://oeis.org/A321770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Computes OEIS sequence A321770 by traversing the tree * of
 *            primitive Pythagorean triples. The sequence enumerates * the
 *            hypotenuses of all triples obtained from the root * [3,4,5]
 *            using the three linear transforms * * A : (a,b,c) → (a-2b+2c,
 *            2a-b+2c, 2a-2b+3c) * B : (a,b,c) → (a+2b+2c, 2a+b+2c, 2...
 * Range:     u8 n <= 17, u16 n <= 485, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* locate row r where cumulative count ≥ n */
    idx_t sum = 0;          /* triples before current row */
    idx_t count = 1;        /* 3^(row-1) */
    idx_t row = 1;

    while (n > sum + count) {
        sum += count;
        count *= 3ULL;
        ++row;
    }
    idx_t k = n - sum - 1;   /* 0‑based index within the row */

    /* initial triple [3, 4, 5] */
    seq_t x = (seq_t)3;
    seq_t y = (seq_t)4;
    seq_t z = (seq_t)5;

    idx_t digits = row - 1;
    if (digits) {
        /* pre‑compute 3^(digits-1) */
        idx_t pow3 = 1;
        for (idx_t i = 1; i < digits; ++i)
            pow3 *= 3ULL;

        idx_t kk = k;
        for (idx_t i = 0; i < digits; ++i) {
            idx_t d = kk / pow3;     /* current base‑3 digit 0,1,2 */
            kk %= pow3;
            pow3 /= 3ULL;

            if (d == 0) {            /* transform A */
                seq_t nx = x + (z << 1);
                nx -= (y << 1);
                seq_t ny = (x << 1) + (z << 1);
                ny -= y;
                seq_t nz = (x << 1) + (z << 1) + z;
                nz -= (y << 1);
                x = nx; y = ny; z = nz;
            } else if (d == 1) {     /* transform B */
                seq_t nx = x + (y << 1) + (z << 1);
                seq_t ny = (x << 1) + y + (z << 1);
                seq_t nz = (x << 1) + (y << 1) + (z << 1) + z;
                x = nx; y = ny; z = nz;
            } else {                 /* transform C */
                seq_t nx = (y << 1) + (z << 1) - x;
                seq_t ny = y + (z << 1) - (x << 1);
                seq_t nz = (y << 1) + (z << 1) + z - (x << 1);
                x = nx; y = ny; z = nz;
            }
        }
    }
    return z;
}
