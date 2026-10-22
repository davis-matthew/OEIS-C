/*
 * A316387  Consider coefficients U(m,L,k) defined by the identity
 *          Sum_{k=1..L} Sum_{j=0..m} A302971(m,j)/A304042(m,j) * k^j *
 *          (T-k)^j = Sum_{k=0..m} (-1)^(m-k) * U(m,L,k) * T^k that holds for
 *          all positive integers L,m,T. This sequence gives 4-column table
 *          read by rows, where the n-th row lists coefficients...
 * https://oeis.org/A316387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   U(3,n,k) polynomials for the 4 coefficients a(4m+1) = 20*n^7 +
 *            70*n^6 + 70*n^5 - 28*n^3 - 7*n^2 a(4m+2) = 70*n^6 + 210*n^5 +
 *            175*n^4 - 42*n^2 - 7*n a(4m+3) = 84*n^5 + 210*n^4 + 140*n^3 -
 *            14*n a(4m+4) = 35*n^4 + 70*n^3 + 35*n^2 where n = m+1 (row
 *            index, 1‑based)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;               /* not part of the sequence */
    idx_t row   = (n - 1) / 4 + 1;   /* row index (m+1) */
    int  col    = (int)((n - 1) % 4);/* column 0..3 */

    seq_t r  = (seq_t)row;
    seq_t n2 = r * r;
    seq_t n3 = n2 * r;
    seq_t n4 = n3 * r;
    seq_t n5 = n4 * r;
    seq_t n6 = n5 * r;
    seq_t n7 = n6 * r;

    seq_t res = 0;

    switch (col) {
    case 0:     /* U(3,row,0) */
        res = 20 * n7;
        res += 70 * n6;
        res += 70 * n5;
        res -= 28 * n3;
        res -= 7 * n2;
        break;
    case 1:     /* U(3,row,1) */
        res = 70 * n6;
        res += 210 * n5;
        res += 175 * n4;
        res -= 42 * n2;
        res -= 7 * r;
        break;
    case 2:     /* U(3,row,2) */
        res = 84 * n5;
        res += 210 * n4;
        res += 140 * n3;
        res -= 14 * r;
        break;
    case 3:     /* U(3,row,3) */
        res = 35 * n4;
        res += 70 * n3;
        res += 35 * n2;
        break;
    }
    return res;
}
