/*
 * A338217  Triangle read by rows: T(n,k) is the coefficient of (1+x)^k in the
 *          ZZ polynomial of the hexagonal graphene flake O(3,3,n).
 * https://oeis.org/A338217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 62, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A338217: flattening of the triangle T(n,k) where
   T(n,k) = binomial(9,k)*binomial(n,k)
         + (10*binomial(7,k-2)-binomial(6,k-2))*binomial(n+1,k)
         + (20*binomial(5,k-4)+binomial(3,k-3)-binomial(3,k-5))*binomial(n+2,k)
         + (10*binomial(3,k-6)+binomial(2,k-5)+binomial(3,k-5))*binomial(n+3,k)
         + binomial(2,k-7)*binomial(n+4,k) .
   The function returns the n‑th element of the triangle read by rows.  */

#include "../utils/binomial.h"

seq_t
a(idx_t n)
{
    /* convert 1‑based index to row/column inside the triangle */
    idx_t idx = n;          /* input index (1‑based) */
    idx_t row, prev_sum;    /* row number and cumulative length of previous rows */

    /* The row lengths are 4, 6, 8, 9, 10,…  (Cl(n) + 1) */
    if (idx <= 4) {
        row = 1;
        prev_sum = 0;
    } else if (idx <= 10) {
        row = 2;
        prev_sum = 4;
    } else if (idx <= 18) {
        row = 3;
        prev_sum = 10;
    } else if (idx <= 27) {
        row = 4;
        prev_sum = 18;
    } else {
        /* rows 5 and onward all have exactly 10 entries */
        row = 5 + (idx - 28) / 10;
        prev_sum = 27 + 10 * (row - 5);
    }

    /* column index within the current row (0‑based) */
    idx_t k = idx - prev_sum - 1;

    /* compute the entry T(row,k) according to the formula */
    seq_t result = 0;

    /* Term 1: binomial(9,k)*binomial(row,k) */
    result += binomial(9, k) * binomial(row, k);

    /* Term 2: (10*binomial(7,k-2)-binomial(6,k-2))*binomial(row+1,k) */
    if (k >= 2) {
        seq_t t = 10 * binomial(7, k - 2) - binomial(6, k - 2);
        result += t * binomial(row + 1, k);
    }

    /* Term 3: (20*binomial(5,k-4)+binomial(3,k-3)-binomial(3,k-5))*binomial(row+2,k) */
    {
        seq_t t = 0;
        if (k >= 4) t += 20 * binomial(5, k - 4);
        if (k >= 3) t += binomial(3, k - 3);
        if (k >= 5) t -= binomial(3, k - 5);
        result += t * binomial(row + 2, k);
    }

    /* Term 4: (10*binomial(3,k-6)+binomial(2,k-5)+binomial(3,k-5))*binomial(row+3,k) */
    {
        seq_t t = 0;
        if (k >= 6) t += 10 * binomial(3, k - 6);
        if (k >= 5) t += binomial(2, k - 5) + binomial(3, k - 5);
        result += t * binomial(row + 3, k);
    }

    /* Term 5: binomial(2,k-7)*binomial(row+4,k) */
    if (k >= 7) {
        result += binomial(2, k - 7) * binomial(row + 4, k);
    }

    return result;
}
