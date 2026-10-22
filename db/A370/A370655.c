/*
 * A370655  Triangle read by rows where row n is a block of length 4*n-1 which
 *          is a permutation of the numbers of its constituents.
 * https://oeis.org/A370655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = P(n) + (L(n)-1)*(2*L(n)-1) where L(n) =
 *            ceil((sqrt(8*n+1)-1)/4) and P(n) is defined in terms of R(n) = n
 *            - (L(n)-1)*(2*L(n)-1) as follows: if R < 2*L-1 and R is odd: P =
 *            2*L - 2 - R if R < 2*L-1 and R is even: P = 2*L - R if R =
 *            2*L-1: P = 2*L if R = 2*L: P = R - 1 if R > 2*L and R is odd:...
 * Range:     u8 n <= 7, u16 n <= 1363, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute L(n) = ceil((sqrt(8*n+1)-1)/4) */
    seq_t sqx = (seq_t)8 * (seq_t)n + 1;     // 8*n + 1
    seq_t s   = floor_sqrt(sqx);             // floor of sqrt(8*n+1)

    seq_t L = (s - 1) / 4;                   // floor division
    /* adjust for ceil((sqrt-1)/4) */
    if ((s - 1) % 4 != 0 || s * s != sqx) {
        ++L;
    }

    /* base offset for the current block */
    seq_t base = (L - 1) * (2 * L - 1);

    /* R = n - base */
    seq_t R = (seq_t)n - base;

    seq_t twoL  = 2 * L;
    seq_t twoLm1 = twoL - 1;
    seq_t P;

    if (R < twoLm1) {          /* R < 2L-1 */
        if (R & 1)             /* odd */
            P = twoL - 2 - R;  /* 2*L - 2 - R */
        else                   /* even */
            P = twoL - R;      /* 2*L - R */
    } else if (R == twoLm1) {  /* R = 2L-1 */
        P = twoL;              /* 2*L */
    } else if (R == twoL) {    /* R = 2L */
        P = R - 1;             /* R - 1 */
    } else {                   /* R > 2L */
        if (R & 1)             /* odd */
            P = R;              /* R */
        else                   /* even */
            P = 6 * L - R;      /* 6*L - R */
    }

    return base + P;
}
