/*
 * A283434  Triangle read by rows: T(n,m) is the number of pattern classes in
 *          the (n,m)-rectangular grid with 5 colors and n>=m, two patterns
 *          are in the same class if one of them can be obtained by a
 *          reflection or 180-degree rotation of the other.
 * https://oeis.org/A283434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r,c are the row and column of the (n+1)-st
 *            element of the triangular indexing, and T(r,c) is given by the
 *            piecewise formulas of OEIS A283434
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find row r and column c for the triangular indexing.
       The first element of row r starts at index start = r*(r+1)/2.  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;        /* column index (0 <= c <= r) */

    /* Basic powers of 5 */
    seq_t prod = (seq_t)r * (seq_t)c;          /* r * c = m * n */
    seq_t pow5 = big_pow((seq_t)5, prod);      /* 5^(m*n)      */
    seq_t half = big_pow((seq_t)5, prod / 2);  /* 5^(m*n/2)    */

    seq_t result = 0;

    if ((r & 1) == 0 && (c & 1) == 0) {          /* even r, even c */
        result = (pow5 + 3 * half) / 4;
    } else if ((r & 1) == 0 && (c & 1) == 1) {   /* even r, odd c */
        seq_t pow_nm = big_pow((seq_t)5, (prod + r) / 2); /* 5^((mn+n)/2) */
        result = (pow5 + pow_nm + 2 * half) / 4;
    } else if ((r & 1) == 1 && (c & 1) == 0) {   /* odd r, even c */
        seq_t pow_cm = big_pow((seq_t)5, (prod + c) / 2); /* 5^((mn+m)/2) */
        result = (pow5 + pow_cm + 2 * half) / 4;
    } else {                                     /* odd r, odd c */
        seq_t pow_nm = big_pow((seq_t)5, (prod + r) / 2);
        seq_t pow_cm = big_pow((seq_t)5, (prod + c) / 2);
        seq_t pow_mid = big_pow((seq_t)5, (prod + 1) / 2);
        result = (pow5 + pow_nm + pow_cm + pow_mid) / 4;
    }

    return result;
}
